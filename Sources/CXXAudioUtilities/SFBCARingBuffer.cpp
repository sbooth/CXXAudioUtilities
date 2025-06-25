//
// Copyright © 2013-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <algorithm>
#import <cassert>
#import <cstdlib>
#import <cstring>
#import <limits>

#import "SFBCARingBuffer.hpp"

namespace {

/// Zeroes a range of bytes in @c buffers
/// @param buffers The destination buffers
/// @param bufferCount The number of buffers
/// @param byteOffset The byte offset in @c buffers to begin writing
/// @param byteCount The number of bytes per non-interleaved buffer to write
void ZeroRange(void * const _Nonnull * const _Nonnull buffers, uint32_t bufferCount, uint32_t byteOffset, uint32_t byteCount)
{
	for(uint32_t i = 0; i < bufferCount; ++i) {
		const auto s = reinterpret_cast<uintptr_t>(buffers[i]) + byteOffset;
		std::memset(reinterpret_cast<void *>(s), 0, byteCount);
	}
}

/// Zeroes a range of bytes in @c bufferList
/// @param bufferList The destination buffers
/// @param byteOffset The byte offset in @c bufferList to begin writing
/// @param byteCount The maximum number of bytes per non-interleaved buffer to write
void ZeroABL(AudioBufferList * const _Nonnull bufferList, uint32_t byteOffset, uint32_t byteCount)
{
	for(UInt32 i = 0; i < bufferList->mNumberBuffers; ++i) {
		assert(byteOffset <= bufferList->mBuffers[i].mDataByteSize);
		const auto s = reinterpret_cast<uintptr_t>(bufferList->mBuffers[i].mData) + byteOffset;
		const auto n = std::min(byteCount, bufferList->mBuffers[i].mDataByteSize - byteOffset);
		std::memset(reinterpret_cast<void *>(s), 0, n);
	}
}

/// Copies non-interleaved audio from @c bufferList to @c buffers
/// @param buffers The destination buffers
/// @param dstOffset The byte offset in @c buffers to begin writing
/// @param bufferList The source buffers
/// @param srcOffset The byte offset in @c bufferList to begin reading
/// @param byteCount The maximum number of bytes per non-interleaved buffer to read and write
void StoreABL(void * const _Nonnull * const _Nonnull buffers, uint32_t dstOffset, const AudioBufferList * const _Nonnull bufferList, uint32_t srcOffset, uint32_t byteCount) noexcept
{
	for(UInt32 i = 0; i < bufferList->mNumberBuffers; ++i) {
		assert(srcOffset <= bufferList->mBuffers[i].mDataByteSize);
		const auto dst = reinterpret_cast<uintptr_t>(buffers[i]) + dstOffset;
		const auto src = reinterpret_cast<uintptr_t>(bufferList->mBuffers[i].mData) + srcOffset;
		const auto n = std::min(byteCount, bufferList->mBuffers[i].mDataByteSize - srcOffset);
		std::memcpy(reinterpret_cast<void *>(dst), reinterpret_cast<const void *>(src), n);
	}
}

/// Copies non-interleaved audio from @c buffers to @c bufferList
/// @param bufferList The destination buffers
/// @param dstOffset The byte offset in @c bufferList to begin writing
/// @param buffers The source buffers
/// @param srcOffset The byte offset in @c bufferList to begin reading
/// @param byteCount The maximum number of bytes per non-interleaved buffer to read and write
void FetchABL(AudioBufferList * const _Nonnull bufferList, uint32_t dstOffset, const void * const _Nonnull * const _Nonnull buffers, uint32_t srcOffset, uint32_t byteCount) noexcept
{
	for(UInt32 i = 0; i < bufferList->mNumberBuffers; ++i) {
		assert(dstOffset <= bufferList->mBuffers[i].mDataByteSize);
		const auto dst = reinterpret_cast<uintptr_t>(bufferList->mBuffers[i].mData) + dstOffset;
		const auto src = reinterpret_cast<uintptr_t>(buffers[i]) + srcOffset;
		const auto n = std::min(byteCount, bufferList->mBuffers[i].mDataByteSize - dstOffset);
		std::memcpy(reinterpret_cast<void *>(dst), reinterpret_cast<const void *>(src), n);
	}
}

/// Returns the smallest power of two value greater than @c x
/// @param x A value in the range [2..2147483648]
/// @return The smallest power of two greater than @c x
constexpr uint32_t NextPowerOfTwo(uint32_t x) noexcept
{
	assert(x > 1);
	assert(x <= ((std::numeric_limits<uint32_t>::max() / 2) + 1));
	return static_cast<uint32_t>(1 << (32 - __builtin_clz(x - 1)));
}

} /* namespace */

#pragma mark Buffer Management

bool SFB::CARingBuffer::Allocate(const CAStreamBasicDescription& format, uint32_t capacityFrames) noexcept
{
	// Only non-interleaved formats are supported
	if(format.IsInterleaved() || capacityFrames < 2 || capacityFrames > 0x80000000)
		return false;

	Deallocate();

	// Round up to the next power of two
	capacityFrames = NextPowerOfTwo(capacityFrames);

	mFormat = format;

	mCapacityFrames = capacityFrames;
	mCapacityFramesMask = capacityFrames - 1;

	const auto capacityBytes = capacityFrames * format.mBytesPerFrame;

	// One memory allocation holds everything- first the pointers followed by the deinterleaved channels
	const auto allocationSize = (capacityBytes + sizeof(void *)) * format.mChannelsPerFrame;
	auto allocation = std::malloc(allocationSize);
	if(!allocation)
		return false;

	// Zero the entire allocation
	std::memset(allocation, 0, allocationSize);

	// Assign the pointers and channel buffers
	auto address = reinterpret_cast<uintptr_t>(allocation);

	mBuffers = reinterpret_cast<void **>(address);
	address += format.mChannelsPerFrame * sizeof(void *);
	for(UInt32 i = 0; i < format.mChannelsPerFrame; ++i) {
		mBuffers[i] = reinterpret_cast<void *>(address);
		address += capacityBytes;
	}

	// Zero the time bounds queue
	for(uint32_t i = 0; i < sTimeBoundsQueueSize; ++i) {
		mTimeBoundsQueue[i].mStartTime = 0;
		mTimeBoundsQueue[i].mEndTime = 0;
		mTimeBoundsQueue[i].mUpdateCounter = 0;
	}

	mTimeBoundsQueueCounter = 0;

	return true;
}

void SFB::CARingBuffer::Deallocate() noexcept
{
	if(mBuffers) {
		std::free(mBuffers);
		mBuffers = nullptr;

		mFormat.Reset();

		mCapacityFrames = 0;
		mCapacityFramesMask = 0;

		for(uint32_t i = 0; i < sTimeBoundsQueueSize; ++i) {
			mTimeBoundsQueue[i].mStartTime = 0;
			mTimeBoundsQueue[i].mEndTime = 0;
			mTimeBoundsQueue[i].mUpdateCounter = 0;
		}

		mTimeBoundsQueueCounter = 0;
	}
}

bool SFB::CARingBuffer::GetTimeBounds(int64_t& startTime, int64_t& endTime) const noexcept
{
	for(auto i = 0; i < 8; ++i) {
		const auto currentCounter = mTimeBoundsQueueCounter.load(std::memory_order_acquire);
		const auto currentIndex = currentCounter & sTimeBoundsQueueMask;

		const SFB::CARingBuffer::TimeBounds * const bounds = mTimeBoundsQueue + currentIndex;

		const auto counter = bounds->mUpdateCounter.load(std::memory_order_acquire);
		if(counter == currentCounter) {
			startTime = bounds->mStartTime;
			endTime = bounds->mEndTime;
			return true;
		}
	}

	return false;
}

#pragma mark Reading and Writing Audio

bool SFB::CARingBuffer::Read(AudioBufferList * const bufferList, uint32_t frameCount, int64_t startRead) noexcept
{
	if(frameCount == 0)
		return true;

	if(!bufferList || frameCount > mCapacityFrames || startRead < 0)
		return false;

	auto endRead = startRead + static_cast<int64_t>(frameCount);

	const auto startRead0 = startRead;
	const auto endRead0 = endRead;

	if(!ClampTimesToBounds(startRead, endRead))
		return false;

	if(startRead == endRead) {
		ZeroABL(bufferList, 0, frameCount * mFormat.mBytesPerFrame);
		return true;
	}

	const auto byteSize = static_cast<uint32_t>(endRead - startRead) * mFormat.mBytesPerFrame;

	const auto destStartByteOffset = static_cast<uint32_t>(std::max(static_cast<int64_t>(0), (startRead - startRead0) * mFormat.mBytesPerFrame));
	if(destStartByteOffset > 0)
		ZeroABL(bufferList, 0, std::min(frameCount * mFormat.mBytesPerFrame, destStartByteOffset));

	const auto destEndSize = static_cast<uint32_t>(std::max(static_cast<int64_t>(0), endRead0 - endRead));
	if(destEndSize > 0)
		ZeroABL(bufferList, destStartByteOffset + byteSize, destEndSize * mFormat.mBytesPerFrame);

	const auto offset0 = FrameByteOffset(startRead);
	const auto offset1 = FrameByteOffset(endRead);
	uint32_t byteCount;

	if(offset0 < offset1) {
		byteCount = offset1 - offset0;
		FetchABL(bufferList, destStartByteOffset, mBuffers, offset0, byteCount);
	}
	else {
		byteCount = static_cast<UInt32>((mCapacityFrames * mFormat.mBytesPerFrame) - offset0);
		FetchABL(bufferList, destStartByteOffset, mBuffers, offset0, byteCount);
		FetchABL(bufferList, destStartByteOffset + byteCount, mBuffers, 0, offset1);
		byteCount += offset1;
	}

	for(UInt32 i = 0; i < bufferList->mNumberBuffers; ++i)
		bufferList->mBuffers[i].mDataByteSize = static_cast<UInt32>(byteCount);

	return true;
}

bool SFB::CARingBuffer::Write(const AudioBufferList * const bufferList, uint32_t frameCount, int64_t startWrite) noexcept
{
	if(frameCount == 0)
		return true;

	if(!bufferList || frameCount > mCapacityFrames || startWrite < 0)
		return false;

	const auto endWrite = startWrite + static_cast<int64_t>(frameCount);

	// Going backwards, throw everything out
	if(startWrite < EndTime())
		SetTimeBounds(startWrite, startWrite);
	// The buffer has not yet wrapped and will not need to
	else if(endWrite - StartTime() <= static_cast<int64_t>(mCapacityFrames))
		;
	// Advance the start time past the region about to be overwritten
	else {
		const int64_t newStart = endWrite - static_cast<int64_t>(mCapacityFrames);	// one buffer of time behind the write position
		const int64_t newEnd = std::max(newStart, EndTime());
		SetTimeBounds(newStart, newEnd);
	}

	uint32_t offset0, offset1;
	const auto curEnd = EndTime();

	if(startWrite > curEnd) {
		// Zero the range of samples being skipped
		offset0 = FrameByteOffset(curEnd);
		offset1 = FrameByteOffset(startWrite);
		if(offset0 < offset1)
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), offset0, offset1 - offset0);
		else {
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), offset0, (mCapacityFrames * mFormat.mBytesPerFrame) - offset0);
			ZeroRange(mBuffers, mFormat.ChannelStreamCount(), 0, offset1);
		}

		offset0 = offset1;
	}
	else
		offset0 = FrameByteOffset(startWrite);

	offset1 = FrameByteOffset(endWrite);
	if(offset0 < offset1)
		StoreABL(mBuffers, offset0, bufferList, 0, offset1 - offset0);
	else {
		auto byteCount = (mCapacityFrames * mFormat.mBytesPerFrame) - offset0;
		StoreABL(mBuffers, offset0, bufferList, 0, byteCount);
		StoreABL(mBuffers, 0, bufferList, byteCount, offset1);
	}

	// Update the end time
	SetTimeBounds(StartTime(), endWrite);

	return true;
}

#pragma mark Internals

void SFB::CARingBuffer::SetTimeBounds(int64_t startTime, int64_t endTime) noexcept
{
	const auto nextCounter = mTimeBoundsQueueCounter.load(std::memory_order_acquire) + 1;
	const auto nextIndex = nextCounter & sTimeBoundsQueueMask;

	mTimeBoundsQueue[nextIndex].mStartTime = startTime;
	mTimeBoundsQueue[nextIndex].mEndTime = endTime;
	mTimeBoundsQueue[nextIndex].mUpdateCounter.store(nextCounter, std::memory_order_release);

	mTimeBoundsQueueCounter.store(nextCounter, std::memory_order_release);
}

bool SFB::CARingBuffer::ClampTimesToBounds(int64_t& startRead, int64_t& endRead) const noexcept
{
	int64_t startTime, endTime;
	if(!GetTimeBounds(startTime, endTime))
		return false;

	if(startRead > endTime || endRead < startTime) {
		endRead = startRead;
		return true;
	}

	startRead = std::max(startRead, startTime);
	endRead = std::max(std::min(endRead, endTime), startRead);

	return true;
}
