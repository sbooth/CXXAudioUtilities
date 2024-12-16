//
// Copyright (c) 2014 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <cstring>
#import <optional>

#import <CoreAudioTypes/CoreAudioTypes.h>

#import "SFBCFWrapper.hpp"

namespace SFB {

/// Common PCM audio formats
enum class CommonPCMFormat {
	/// Native-endian @c float
	float32,
	/// Native-endian @c double
	float64,
	/// Native-endian @c int16_t
	int16,
	/// Native-endian @c int32_t
	int32,
};

/// A class extending the functionality of a Core Audio @c AudioStreamBasicDescription
class CAStreamBasicDescription : public AudioStreamBasicDescription
{

public:
	
#pragma mark Creation and Destruction

	/// Creates an empty @c CAStreamBasicDescription
	CAStreamBasicDescription() noexcept = default;

	/// Copy constructor
	CAStreamBasicDescription(const CAStreamBasicDescription& rhs) noexcept = default;

	/// Assignment operator
	CAStreamBasicDescription& operator=(const CAStreamBasicDescription& rhs) noexcept = default;

	/// Destructor
	~CAStreamBasicDescription() = default;

	/// Move constructor
	CAStreamBasicDescription(CAStreamBasicDescription&& rhs) noexcept = default;

	/// Move assignment operator
	CAStreamBasicDescription& operator=(CAStreamBasicDescription&& rhs) noexcept = default;


	/// Creates a new @c CAStreamBasicDescription for the speciifed @c CommonPCMFormat
	CAStreamBasicDescription(CommonPCMFormat commonPCMFormat, Float64 sampleRate, UInt32 channelsPerFrame, bool isInterleaved) noexcept
	: AudioStreamBasicDescription{}
	{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
		switch(commonPCMFormat) {
			case CommonPCMFormat::float32:
				FillOutASBDForLPCM(*this, sampleRate, channelsPerFrame, 32, 32, true, kAudioFormatFlagIsBigEndian == kAudioFormatFlagsNativeEndian, !isInterleaved);
				break;
			case CommonPCMFormat::float64:
				FillOutASBDForLPCM(*this, sampleRate, channelsPerFrame, 64, 64, true, kAudioFormatFlagIsBigEndian == kAudioFormatFlagsNativeEndian, !isInterleaved);
				break;
			case CommonPCMFormat::int16:
				FillOutASBDForLPCM(*this, sampleRate, channelsPerFrame, 16, 16, false, kAudioFormatFlagIsBigEndian == kAudioFormatFlagsNativeEndian, !isInterleaved);
				break;
			case CommonPCMFormat::int32:
				FillOutASBDForLPCM(*this, sampleRate, channelsPerFrame, 32, 32, false, kAudioFormatFlagIsBigEndian == kAudioFormatFlagsNativeEndian, !isInterleaved);
				break;
		}
#pragma clang diagnostic pop
	}

	// Native overloads

	/// Creates a new @c CAStreamBasicDescription for the specified @c AudioStreamBasicDescription
	CAStreamBasicDescription(const AudioStreamBasicDescription& rhs) noexcept
	: AudioStreamBasicDescription{rhs}
	{}

	/// Assignment operator
	CAStreamBasicDescription& operator=(const AudioStreamBasicDescription& rhs) noexcept
	{
		AudioStreamBasicDescription::operator=(rhs);
		return *this;
	}

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	bool operator==(const AudioStreamBasicDescription& rhs) const noexcept
	{
		return !std::memcmp(this, &rhs, sizeof(AudioStreamBasicDescription));
	}

	/// Returns @c true if @c rhs is not equal to @c this
	bool operator!=(const AudioStreamBasicDescription& rhs) const noexcept
	{
		return !operator==(rhs);
	}

#pragma mark Format information

	/// Returns the common PCM format described by @c this or @c std::nullopt if none
	std::optional<CommonPCMFormat> GetCommonFormat() const noexcept;

	/// Returns @c true if this format is non-interleaved
	bool IsNonInterleaved() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonInterleaved) == kAudioFormatFlagIsNonInterleaved;
	}

	/// Returns @c true if this format is interleaved
	bool IsInterleaved() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonInterleaved) == 0;
	}

	/// Returns the number of interleaved channels
	UInt32 InterleavedChannelCount() const noexcept
	{
		return IsInterleaved() ? mChannelsPerFrame : 1;
	}

	/// Returns the number of channel streams
	UInt32 ChannelStreamCount() const noexcept
	{
		return IsInterleaved() ? 1 : mChannelsPerFrame;
	}

	/// Returns the number of channels
	UInt32 ChannelCount() const noexcept
	{
		return mChannelsPerFrame;
	}

	/// Returns @c true if this format is PCM
	bool IsPCM() const noexcept
	{
		return mFormatID == kAudioFormatLinearPCM;
	}

	/// Returns @c true if this format is big-endian
	bool IsBigEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagIsBigEndian;
	}

	/// Returns @c true if this format is little-endian
	bool IsLittleEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == 0;
	}

	/// Returns @c true if this format is native-endian
	bool IsNativeEndian() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian;
	}

	/// Returns @c true if this format is floating-point linear PCM
	bool IsFloat() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsFloat) == kAudioFormatFlagIsFloat;
	}

	/// Returns @c true if this format is integer linear PCM
	bool IsInteger() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsFloat) == 0;
	}

	/// Returns @c true if this format is signed integer linear PCM
	bool IsSignedInteger() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsSignedInteger) == kAudioFormatFlagIsSignedInteger;
	}

	/// Returns @c true if this format is packed
	///
	/// A format is considered packed even if @c kAudioFormatFlagIsPacked is clear when  @c ((mBitsPerChannel/8)*mChannelsPerFrame)==mBytesPerFrame
	bool IsPacked() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsPacked) == kAudioFormatFlagIsPacked || ((mBitsPerChannel / 8) * mChannelsPerFrame) == mBytesPerFrame;
	}

	/// Returns @c true if this format is high-aligned
	bool IsAlignedHigh() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsAlignedHigh) == kAudioFormatFlagIsAlignedHigh;
	}

	/// Returns the number of fractional bits
	UInt32 FractionalBits() const noexcept
	{
		return (mFormatFlags & kLinearPCMFormatFlagsSampleFractionMask) >> kLinearPCMFormatFlagsSampleFractionShift;
	}

	/// Returns @c true if this format is integer fixed-point PCM
	bool IsFixedPoint() const noexcept
	{
		return IsInteger() && FractionalBits() > 0;
	}

	/// Returns @c true if this format is non-mixable
	/// @note This flag is only used when interacting with HAL stream formats
	bool IsNonMixable() const noexcept
	{
		return (mFormatFlags & kAudioFormatFlagIsNonMixable) == kAudioFormatFlagIsNonMixable;
	}

	/// Returns @c true if this format is mixable
	/// @note This flag is only used when interacting with HAL stream formats
	bool IsMixable() const noexcept
	{
		return IsPCM() && (mFormatFlags & kAudioFormatFlagIsNonMixable) == 0;
	}

	/// Returns the sample word size in bytes
	UInt32 SampleWordSize() const noexcept
	{
		auto interleavedChannelCount = InterleavedChannelCount();
		if(!interleavedChannelCount)
			return 0;
		return mBytesPerFrame / interleavedChannelCount;
	}

	/// Returns the byte size of @c frameCount audio frames
	/// @note This is equivalent to @c frameCount*mBytesPerFrame
	UInt32 FrameCountToByteSize(UInt32 frameCount) const noexcept
	{
		return frameCount * mBytesPerFrame;
	}

	/// Returns the frame count of @c byteSize bytes
	/// @note This is equivalent to @c byteSize/mBytesPerFrame
	UInt32 ByteSizeToFrameCount(UInt32 byteSize) const noexcept
	{
		if(!mBytesPerFrame)
			return 0;
		return byteSize / mBytesPerFrame;
	}

	/// Returns the duration of a single packet in seconds
	double GetPacketDuration() const noexcept
	{
		return (1 / mSampleRate) * mFramesPerPacket;
	}

#pragma mark Format transformation

	/// Sets @c format to the equivalent non-interleaved format of @c this. Fails for non-PCM formats.
	bool GetNonInterleavedEquivalent(CAStreamBasicDescription& format) const noexcept
	{
		if(!IsPCM() || !mChannelsPerFrame)
			return false;
		format = *this;
		if(IsInterleaved()) {
			format.mFormatFlags |= kAudioFormatFlagIsNonInterleaved;
			format.mBytesPerPacket /= mChannelsPerFrame;
			format.mBytesPerFrame /= mChannelsPerFrame;
		}
		return true;
	}

	/// Sets @c format to the equivalent interleaved format of @c this. Fails for non-PCM formats.
	bool GetInterleavedEquivalent(CAStreamBasicDescription& format) const noexcept
	{
		if(!IsPCM())
			return false;
		format = *this;
		if(!IsInterleaved()) {
			format.mFormatFlags &= ~kAudioFormatFlagIsNonInterleaved;
			format.mBytesPerPacket *= mChannelsPerFrame;
			format.mBytesPerFrame *= mChannelsPerFrame;
		}
		return true;
	}

	/// Sets @c format to the equivalent standard format of @c this. Fails for non-PCM formats.
	bool GetStandardEquivalent(CAStreamBasicDescription& format) const noexcept
	{
		if(!IsPCM())
			return false;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
		FillOutASBDForLPCM(format, mSampleRate, mChannelsPerFrame, 32, 32, true, kAudioFormatFlagIsBigEndian == kAudioFormatFlagsNativeEndian, true);
#pragma clang diagnostic pop
		return true;
	}

	/// Resets the @c CAStreamBasicDescription to the default state
	void Reset() noexcept
	{
		std::memset(this, 0, sizeof(AudioStreamBasicDescription));
	}


	/// Returns a string representation of this format suitable for logging
	CFString Description() const noexcept;

};

} // namespace SFB
