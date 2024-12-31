//
// Copyright © 2013-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <algorithm>
#import <cstdlib>
#import <limits>
#import <new>

#import "SFBCABufferList.hpp"
#import "SFBCAAudioFile.hpp"
#import "SFBCAExtAudioFile.hpp"
//#import "SFBCAAudioConverter.hpp"

AudioBufferList * SFB::AllocateAudioBufferList(const CAStreamBasicDescription& format, UInt32 frameCapacity) noexcept
{
	if(format.mBytesPerFrame == 0 || frameCapacity > (std::numeric_limits<UInt32>::max() / format.mBytesPerFrame))
		return nullptr;

	const auto bufferDataSize = format.FrameCountToByteSize(frameCapacity);
	const auto bufferCount = format.ChannelStreamCount();
	const auto bufferListSize = offsetof(AudioBufferList, mBuffers) + (sizeof(AudioBuffer) * bufferCount);
	const auto allocationSize = bufferListSize + (bufferDataSize * bufferCount);

	auto allocation = std::malloc(allocationSize);
	if(!allocation)
		return nullptr;

	// Zero the entire allocation
	std::memset(allocation, 0, allocationSize);

	// Assign the buffers
	auto address = reinterpret_cast<uintptr_t>(allocation);

	auto abl = reinterpret_cast<AudioBufferList *>(address);
	abl->mNumberBuffers = bufferCount;

	for(UInt32 i = 0; i < bufferCount; ++i) {
		abl->mBuffers[i].mNumberChannels = format.InterleavedChannelCount();
		abl->mBuffers[i].mData = reinterpret_cast<void *>(address + bufferListSize + (bufferDataSize * i));
		abl->mBuffers[i].mDataByteSize = bufferDataSize;
	}

	return abl;
}

SFB::CABufferList::CABufferList(CABufferList&& rhs) noexcept
: mBufferList{rhs.mBufferList}, mFormat{rhs.mFormat}, mFrameCapacity{rhs.mFrameCapacity}, mFrameLength{rhs.mFrameLength}
{
	rhs.mBufferList = nullptr;
	rhs.mFormat.Reset();
	rhs.mFrameCapacity = 0;
	rhs.mFrameLength = 0;
}

SFB::CABufferList& SFB::CABufferList::operator=(CABufferList&& rhs) noexcept
{
	if(this != &rhs) {
		Deallocate();

		mBufferList = rhs.mBufferList;
		mFormat = rhs.mFormat;
		mFrameCapacity = rhs.mFrameCapacity;
		mFrameLength = rhs.mFrameLength;

		rhs.mBufferList = nullptr;
		rhs.mFormat.Reset();
		rhs.mFrameCapacity = 0;
		rhs.mFrameLength = 0;
	}

	return *this;
}

SFB::CABufferList::CABufferList(const CAStreamBasicDescription& format, UInt32 frameCapacity)
: CABufferList{}
{
	if(format.mBytesPerFrame == 0)
		throw std::invalid_argument("format.mBytesPerFrame == 0");
	if(!Allocate(format, frameCapacity))
		throw std::bad_alloc();
}

#pragma mark Buffer Management

bool SFB::CABufferList::Allocate(const CAStreamBasicDescription& format, UInt32 frameCapacity) noexcept
{
	if(mBufferList)
		Deallocate();

	mBufferList = AllocateAudioBufferList(format, frameCapacity);
	if(!mBufferList)
		return false;

	mFormat = format;
	mFrameCapacity = frameCapacity;
	mFrameLength = 0;

	return true;
}

void SFB::CABufferList::Deallocate() noexcept
{
	if(mBufferList) {
		std::free(mBufferList);
		mBufferList = nullptr;

		mFormat.Reset();

		mFrameCapacity = 0;
		mFrameLength = 0;
	}
}

bool SFB::CABufferList::SetFrameLength(UInt32 frameLength) noexcept
{
	if(!mBufferList || frameLength > mFrameCapacity)
		return false;

	mFrameLength = frameLength;

	for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i)
		mBufferList->mBuffers[i].mDataByteSize = mFrameLength * mFormat.mBytesPerFrame;

	return true;
}

bool SFB::CABufferList::InferFrameLengthFromABL()
{
	if(!mBufferList)
		return false;

	const auto buffer0ByteSize = mBufferList->mBuffers[0].mDataByteSize;
	for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i) {
		if(mBufferList->mBuffers[i].mDataByteSize != buffer0ByteSize)
			throw std::logic_error("inconsistent values for mBufferList->mBuffers[].mBytesPerFrame");
	}

	const auto frameLength = buffer0ByteSize / mFormat.mBytesPerFrame;
	if(frameLength > mFrameCapacity)
		throw std::logic_error("mBufferList->mBuffers[0].mBytesPerFrame / mFormat.mBytesPerFrame > mFrameCapacity");

	mFrameLength = frameLength;

	return true;
}

#pragma mark Buffer Utilities

UInt32 SFB::CABufferList::InsertFromBuffer(const CABufferList& buffer, UInt32 readOffset, UInt32 frameLength, UInt32 writeOffset) noexcept
{
	if(mFormat != buffer.mFormat)
//		throw std::invalid_argument("mFormat != buffer.mFormat");
		return 0;

	if(readOffset > buffer.mFrameLength || writeOffset > mFrameLength || frameLength == 0 || buffer.mFrameLength == 0)
		return 0;

	const auto framesToInsert = std::min(mFrameCapacity - mFrameLength, std::min(frameLength, buffer.mFrameLength - readOffset));

	const auto framesToMove = mFrameLength - writeOffset;
	if(framesToMove) {
		auto moveToOffset = writeOffset + framesToInsert;
		for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i) {
			const auto data = reinterpret_cast<uintptr_t>(mBufferList->mBuffers[i].mData);
			std::memmove(reinterpret_cast<void *>(data + (moveToOffset * mFormat.mBytesPerFrame)),
						 reinterpret_cast<const void *>(data + (writeOffset * mFormat.mBytesPerFrame)),
						 framesToMove * mFormat.mBytesPerFrame);
		}
	}

	if(framesToInsert) {
		for(UInt32 i = 0; i < buffer.mBufferList->mNumberBuffers; ++i)
			std::memcpy(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(mBufferList->mBuffers[i].mData) + (writeOffset * mFormat.mBytesPerFrame)),
						reinterpret_cast<const void *>(reinterpret_cast<uintptr_t>(buffer.mBufferList->mBuffers[i].mData) + (readOffset * mFormat.mBytesPerFrame)),
						framesToInsert * mFormat.mBytesPerFrame);

		SetFrameLength(mFrameLength + framesToInsert);
	}

	return framesToInsert;
}

UInt32 SFB::CABufferList::TrimAtOffset(UInt32 offset, UInt32 frameLength) noexcept
{
	if(offset > mFrameLength || frameLength == 0)
		return 0;

	const auto framesToTrim = std::min(frameLength, mFrameLength - offset);

	const auto framesToMove = mFrameLength - (offset + framesToTrim);
	if(framesToMove) {
		auto moveFromOffset = offset + framesToTrim;
		for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i) {
			const auto data = reinterpret_cast<uintptr_t>(mBufferList->mBuffers[i].mData);
			std::memmove(reinterpret_cast<void *>(data + (offset * mFormat.mBytesPerFrame)),
						 reinterpret_cast<const void *>(data + (moveFromOffset * mFormat.mBytesPerFrame)),
						 framesToMove * mFormat.mBytesPerFrame);
		}
	}

	SetFrameLength(mFrameLength - framesToTrim);

	return framesToTrim;
}

UInt32 SFB::CABufferList::InsertSilence(UInt32 offset, UInt32 frameLength) noexcept
{
	if(!(mFormat.IsFloat() || mFormat.IsSignedInteger()))
//		throw std::logic_error("Inserting silence for unsigned integer samples not supported");
		return 0;

	if(offset > mFrameLength || frameLength == 0)
		return 0;

	const auto framesToZero = std::min(mFrameCapacity - mFrameLength, frameLength);

	const auto framesToMove = mFrameLength - offset;
	if(framesToMove) {
		auto moveToOffset = offset + framesToZero;
		for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i) {
			const auto data = reinterpret_cast<uintptr_t>(mBufferList->mBuffers[i].mData);
			std::memmove(reinterpret_cast<void *>(data + (moveToOffset * mFormat.mBytesPerFrame)),
						 reinterpret_cast<const void *>(data + (offset * mFormat.mBytesPerFrame)),
						 framesToMove * mFormat.mBytesPerFrame);
		}
	}

	if(framesToZero) {
		// For floating-point numbers this code is non-portable: the C standard doesn't require IEEE 754 compliance
		// However, setting all bits to 0 using memset() on macOS results in a floating-point value of 0
		for(UInt32 i = 0; i < mBufferList->mNumberBuffers; ++i) {
			auto s = reinterpret_cast<uintptr_t>(mBufferList->mBuffers[i].mData) + (offset * mFormat.mBytesPerFrame);
			std::memset(reinterpret_cast<void *>(s), 0, framesToZero * mFormat.mBytesPerFrame);
		}

		SetFrameLength(mFrameLength + framesToZero);
	}

	return framesToZero;
}

bool SFB::CABufferList::AdoptABL(AudioBufferList *bufferList, const AudioStreamBasicDescription& format, UInt32 frameCapacity, UInt32 frameLength) noexcept
{
	if(!bufferList || frameLength > frameCapacity)
		return false;

	Deallocate();

	mBufferList = bufferList;
	mFormat = format;
	mFrameCapacity = frameCapacity;
	SetFrameLength(frameLength);

	return true;
}

AudioBufferList * SFB::CABufferList::RelinquishABL() noexcept
{
	auto bufferList = mBufferList;

	mBufferList = nullptr;
	mFormat.Reset();
	mFrameCapacity = 0;
	mFrameLength = 0;

	return bufferList;
}
