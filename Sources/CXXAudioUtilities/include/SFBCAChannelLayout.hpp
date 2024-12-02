//
// Copyright © 2013-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <utility>
#import <vector>

#import <CoreAudioTypes/CoreAudioTypes.h>

#ifdef __OBJC__
#import <AVFAudio/AVFAudio.h>
#endif /* __OBJC__ */

#import "SFBCFWrapper.hpp"

namespace SFB {

/// Returns the size of an @c AudioChannelLayout struct
/// @param channelLayout A pointer to an @c AudioChannelLayout struct
/// @return The size of @c channelLayout in bytes
size_t AudioChannelLayoutSize(const AudioChannelLayout * _Nullable channelLayout) noexcept;

/// A class wrapping a Core Audio @c AudioChannelLayout
class CAChannelLayout
{

public:

	/// Mono layout
	static const CAChannelLayout Mono;

	/// Stereo layout
	static const CAChannelLayout Stereo;

#pragma mark Factory Methods

	/// Creates a @c CAChannelLayout
	/// @param channelBitmap The channel bitmap for the channel layout
	/// @return A @c CAChannelLayout
	static CAChannelLayout ChannelLayoutWithBitmap(AudioChannelBitmap channelBitmap);

	/// Creates a @c CAChannelLayout
	/// @param layoutTag The layout tag for the channel layout
	/// @return A @c CAChannelLayout
	static CAChannelLayout ChannelLayoutWithTag(AudioChannelLayoutTag layoutTag);

	/// Creates a @c CAChannelLayout
	/// @param channelLabels A @c std::vector of the desired channel labels
	/// @return A @c CAChannelLayout
	static CAChannelLayout ChannelLayoutWithChannelLabels(std::vector<AudioChannelLabel> channelLabels);

#pragma mark Creation and Destruction

	/// Creates an empty @c CAChannelLayout
	CAChannelLayout() noexcept = default;

	/// Copy constructor
	CAChannelLayout(const CAChannelLayout& rhs);

	/// Assignment operator
	CAChannelLayout& operator=(const CAChannelLayout& rhs);

	/// Destroys the @c CAChannelLayout and release all associated resources.
	inline ~CAChannelLayout()
	{
		Reset();
	}

	/// Move constructor
	CAChannelLayout(CAChannelLayout&& rhs) noexcept
	: mChannelLayout{rhs.Release()}
	{}

	/// Move assignment operator
	CAChannelLayout& operator=(CAChannelLayout&& rhs) noexcept
	{
		if(this != &rhs)
			Reset(rhs.Release());
		return *this;
	}


	/// Creates a @c CAChannelLayout
	/// @param layoutTag The layout tag for the channel layout
	CAChannelLayout(AudioChannelLayoutTag layoutTag);

	/// Creates a @c CAChannelLayout
	/// @param channelLabels A @c std::vector of the desired channel labels
	CAChannelLayout(std::vector<AudioChannelLabel> channelLabels);

	// Native overloads

	/// Creates a new @c CAChannelLayout by performing a deep copy of @c rhs
	CAChannelLayout(const AudioChannelLayout * _Nullable rhs);

	/// Performs a deep copy of @c rhs
	CAChannelLayout& operator=(const AudioChannelLayout * _Nullable rhs);

#pragma mark Comparison

	/// Returns @c true if @c rhs is equal to @c this
	bool operator==(const CAChannelLayout& rhs) const noexcept;

	/// Returns @c true if @c rhs is not equal to @c this
	inline bool operator!=(const CAChannelLayout& rhs) const noexcept
	{
		return !operator==(rhs);
	}

#pragma mark Functionality

	/// Returns the number of channels contained in this channel layout
	UInt32 ChannelCount() const noexcept;

	/// Creates a channel map for remapping audio from this channel layout
	/// @param outputLayout The output channel layout
	/// @param channelMap A @c std::vector to receive the channel map on success
	/// @return @c true on success, @c false otherwise
	bool MapToLayout(const CAChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const;

#pragma mark AudioChannelLayout access

	/// Returns the size in bytes of this object's internal @c AudioChannelLayout
	inline size_t Size() const noexcept
	{
		return AudioChannelLayoutSize(mChannelLayout);
	}

	/// Releases ownership of the object's internal @c AudioChannelLayout and returns it
	/// @note The caller assumes responsiblity for deallocating the returned @c AudioChannelLayout using @c std::free
	inline AudioChannelLayout * _Nullable Release() noexcept
	{
		return std::exchange(mChannelLayout, nullptr);
	}

	/// Replaces the object's internal @c AudioChannelLayout with @c channelLayout and then deallocates it
	/// @note The object assumes responsiblity for deallocating the passed @c AudioChannelLayout using @c std::free
	inline void Reset(AudioChannelLayout * _Nullable channelLayout = nullptr) noexcept
	{
		std::free(std::exchange(mChannelLayout, channelLayout));
	}

	/// Retrieves a const pointer to this object's internal @c AudioChannelLayout
	inline const AudioChannelLayout * _Nullable ChannelLayout() const noexcept
	{
		return mChannelLayout;
	}


	/// Returns @c true if this object's internal @c AudioChannelLayout is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mChannelLayout != nullptr;
	}

	/// Returns @c true if this object's internal @c AudioChannelLayout is @c nullptr
	inline bool operator!() const noexcept
	{
		return !operator bool();
	}


	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	inline const AudioChannelLayout * _Nullable operator->() const noexcept
	{
		return mChannelLayout;
	}

	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	inline operator const AudioChannelLayout * const _Nullable () const noexcept
	{
		return mChannelLayout;
	}


	/// Returns a string representation of this channel layout suitable for logging
	CFString Description(const char * const _Nullable prefix = nullptr) const noexcept;


#ifdef __OBJC__
	/// Returns an  @c AVAudioChannelLayout object initialized with this object's internal @c AudioChannelLayout
	inline operator AVAudioChannelLayout * _Nullable () const noexcept
	{
		return [[AVAudioChannelLayout alloc] initWithLayout:mChannelLayout];
	}
#endif /* __OBJC__ */

private:

	/// The underlying @c AudioChannelLayout struct
	AudioChannelLayout * _Nullable mChannelLayout = nullptr;

};

} // namespace SFB
