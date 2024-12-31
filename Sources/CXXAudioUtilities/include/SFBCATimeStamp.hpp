//
// Copyright © 2021-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <cstring>

#import <CoreAudioTypes/CoreAudioTypes.h>

namespace SFB {

/// A class extending the functionality of a Core Audio @c AudioTimeStamp
class CATimeStamp : public AudioTimeStamp
{

public:

#pragma mark Creation and Destruction

	/// Creates an empty @c CATimeStamp
	constexpr CATimeStamp() noexcept
	: AudioTimeStamp{0}
	{}

	/// Copy constructor
	CATimeStamp(const CATimeStamp& rhs) noexcept = default;

	/// Assignment operator
	CATimeStamp& operator=(const CATimeStamp& rhs) noexcept = default;

	/// Destructor
	~CATimeStamp() = default;

	/// Move constructor
	CATimeStamp(CATimeStamp&& rhs) noexcept = default;

	/// Move assignment operator
	CATimeStamp& operator=(CATimeStamp&& rhs) noexcept = default;


	/// Creates a new @c CATimeStamp with the specified sample time
	CATimeStamp(Float64 sampleTime) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mFlags = kAudioTimeStampSampleTimeValid;
	}

	/// Creates a new @c CATimeStamp with the specified host time
	CATimeStamp(UInt64 hostTime) noexcept
	: AudioTimeStamp{}
	{
		mHostTime = hostTime;
		mFlags = kAudioTimeStampHostTimeValid;
	}

	/// Creates a new @c CATimeStamp with the specified sample and host times
	CATimeStamp(Float64 sampleTime, UInt64 hostTime) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid;
	}

	/// Creates a new @c CATimeStamp with the specified sample and host times and rate scalar
	CATimeStamp(Float64 sampleTime, UInt64 hostTime, Float64 rateScalar) noexcept
	: AudioTimeStamp{}
	{
		mSampleTime = sampleTime;
		mHostTime = hostTime;
		mRateScalar = rateScalar;
		mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid | kAudioTimeStampRateScalarValid;
	}

	// Native overloads

	/// Creates a new @c CATimeStamp for the specified @c AudioTimeStamp
	CATimeStamp(const AudioTimeStamp& rhs) noexcept
	: AudioTimeStamp{rhs}
	{}

	/// Assignment operator
	CATimeStamp& operator=(const AudioTimeStamp& rhs) noexcept
	{
		AudioTimeStamp::operator=(rhs);
		return *this;
	}

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	bool operator==(const AudioTimeStamp& rhs) const noexcept
	{
		if(SampleTimeIsValid() && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
			return mSampleTime == rhs.mSampleTime;

		if(HostTimeIsValid() && (rhs.mFlags & kAudioTimeStampHostTimeValid))
			return mHostTime == rhs.mHostTime;

		if(WordClockTimeIsValid() && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
			return mWordClockTime == rhs.mWordClockTime;

		return false;
	}

	/// Returns @c true if @c rhs is not equal to @c this
	bool operator!=(const AudioTimeStamp& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	/// Returns @c true if @c rhs is less than @c this
	bool operator<(const AudioTimeStamp& rhs) const noexcept
	{
		if(SampleTimeIsValid() && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
			return mSampleTime < rhs.mSampleTime;

		if(HostTimeIsValid() && (rhs.mFlags & kAudioTimeStampHostTimeValid))
			return mHostTime < rhs.mHostTime;

		if(WordClockTimeIsValid() && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
			return mWordClockTime < rhs.mWordClockTime;

		return false;
	}

	/// Returns @c true if @c rhs is less than or equal to @c this
	bool operator<=(const AudioTimeStamp& rhs) const noexcept
	{
		return !operator>(rhs);
	}

	/// Returns @c true if @c rhs is greater than or equal to @c this
	bool operator>=(const AudioTimeStamp& rhs) const noexcept
	{
		return !operator<(rhs);
	}

	/// Returns @c true if @c rhs is greater than @c this
	bool operator>(const AudioTimeStamp& rhs) const noexcept
	{
		if(SampleTimeIsValid() && (rhs.mFlags & kAudioTimeStampSampleTimeValid))
			return mSampleTime > rhs.mSampleTime;

		if(HostTimeIsValid() && (rhs.mFlags & kAudioTimeStampHostTimeValid))
			return mHostTime > rhs.mHostTime;

		if(WordClockTimeIsValid() && (rhs.mFlags & kAudioTimeStampWordClockTimeValid))
			return mWordClockTime > rhs.mWordClockTime;

		return false;
	}

#pragma mark Flags

	/// Returns @c true if the @c CATimeStamp is valid
	explicit operator bool() const noexcept
	{
		return mFlags != kAudioTimeStampNothingValid;
	}

	/// Returns @c true if the @c CATimeStamp is valid
	bool IsValid() const noexcept
	{
		return operator bool();
	}

	/// Returns @c true if @c mSampleTime is valid
	bool SampleTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSampleTimeValid) == kAudioTimeStampSampleTimeValid;
	}

	/// Returns @c true if @c mHostTime is valid
	bool HostTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampHostTimeValid) == kAudioTimeStampHostTimeValid;
	}

	/// Returns @c true if @c mRateScalar is valid
	bool RateScalarIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampRateScalarValid) == kAudioTimeStampRateScalarValid;
	}

	/// Returns @c true if @c mWordClockTime is valid
	bool WordClockTimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampWordClockTimeValid) == kAudioTimeStampWordClockTimeValid;
	}

	/// Returns @c true if @c mSMPTETime is valid
	bool SMPTETimeIsValid() const noexcept
	{
		return (mFlags & kAudioTimeStampSMPTETimeValid) == kAudioTimeStampSMPTETimeValid;
	}

};

} // namespace SFB
