//
// Copyright © 2013-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <utility>
#import <vector>

#import <CoreAudioTypes/CoreAudioTypes.h>
#import <CoreFoundation/CFString.h>

#ifdef __OBJC__
#import <AVFAudio/AVFAudio.h>
#import <Foundation/NSString.h>
#endif /* __OBJC__ */

namespace SFB {

/// Returns the size of an @c AudioChannelLayout struct
/// @param channelLayout A pointer to an @c AudioChannelLayout struct
/// @return The size of @c channelLayout in bytes
size_t AudioChannelLayoutSize(const AudioChannelLayout * _Nullable channelLayout) noexcept;

/// Returns the name of @c channelLayout
///
/// This is the value of @c kAudioFormatProperty_ChannelLayoutName or @c kAudioFormatProperty_ChannelLayoutSimpleName
/// - note: The caller is responsible for releasing the returned string
CFStringRef _Nullable CopyAudioChannelLayoutName(const AudioChannelLayout * _Nullable channelLayout, bool simpleName = false) noexcept CF_RETURNS_RETAINED;

/// Returns a string representation of @c channelLayout
/// - note: The caller is responsible for releasing the returned string
CFStringRef _Nullable CopyAudioChannelLayoutDescription(const AudioChannelLayout * _Nullable channelLayout) noexcept CF_RETURNS_RETAINED;

#ifdef __OBJC__
/// Returns the name of @c channelLayout
///
/// This is the value of @c kAudioFormatProperty_ChannelLayoutName or @c kAudioFormatProperty_ChannelLayoutSimpleName
inline NSString * _Nullable AudioChannelLayoutName(const AudioChannelLayout * _Nullable channelLayout, bool simpleName = false) noexcept
{
	return (__bridge_transfer NSString *)CopyAudioChannelLayoutName(channelLayout, simpleName);
}

/// Returns a string representation of @c channelLayout
inline NSString * _Nullable AudioChannelLayoutDescription(const AudioChannelLayout * _Nullable channelLayout) noexcept
{
	return (__bridge_transfer NSString *)CopyAudioChannelLayoutDescription(channelLayout);
}
#endif /* __OBJC__ */

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
	constexpr CAChannelLayout() noexcept = default;

	/// Copy constructor
	CAChannelLayout(const CAChannelLayout& rhs);

	/// Assignment operator
	CAChannelLayout& operator=(const CAChannelLayout& rhs);

	/// Destroys the @c CAChannelLayout and release all associated resources.
	~CAChannelLayout()
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
	bool operator!=(const CAChannelLayout& rhs) const noexcept
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
	size_t Size() const noexcept
	{
		return AudioChannelLayoutSize(mChannelLayout);
	}

	/// Releases ownership of the object's internal @c AudioChannelLayout and returns it
	/// @note The caller assumes responsiblity for deallocating the returned @c AudioChannelLayout using @c std::free
	AudioChannelLayout * _Nullable Release() noexcept
	{
		return std::exchange(mChannelLayout, nullptr);
	}

	/// Replaces the object's internal @c AudioChannelLayout with @c channelLayout and then deallocates it
	/// @note The object assumes responsiblity for deallocating the passed @c AudioChannelLayout using @c std::free
	void Reset(AudioChannelLayout * _Nullable channelLayout = nullptr) noexcept
	{
		std::free(std::exchange(mChannelLayout, channelLayout));
	}

	/// Retrieves a const pointer to this object's internal @c AudioChannelLayout
	const AudioChannelLayout * _Nullable ChannelLayout() const noexcept
	{
		return mChannelLayout;
	}


	/// Returns @c true if this object's internal @c AudioChannelLayout is not @c nullptr
	explicit operator bool() const noexcept
	{
		return mChannelLayout != nullptr;
	}


	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	const AudioChannelLayout * _Nullable operator->() const noexcept
	{
		return mChannelLayout;
	}

	/// Retrieve a const pointer to this object's internal @c AudioChannelLayout
	operator const AudioChannelLayout * const _Nullable () const noexcept
	{
		return mChannelLayout;
	}


	/// Returns the name of this channel layout
	///
	/// This is the value of @c kAudioFormatProperty_ChannelLayoutName or @c kAudioFormatProperty_ChannelLayoutSimpleName
	/// - note: The caller is responsible for releasing the returned string
	CFStringRef _Nullable CopyLayoutName(bool simpleName = false) const noexcept CF_RETURNS_RETAINED
	{
		return CopyAudioChannelLayoutName(mChannelLayout, simpleName);
	}

	/// Returns a string representation of this channel layout
	/// - note: The caller is responsible for releasing the returned string
	CFStringRef _Nullable CopyLayoutDescription() const noexcept CF_RETURNS_RETAINED
	{
		return CopyAudioChannelLayoutDescription(mChannelLayout);
	}


#ifdef __OBJC__
	/// Returns an @c AVAudioChannelLayout object initialized with this object's internal @c AudioChannelLayout
	operator AVAudioChannelLayout * _Nullable () const noexcept
	{
		return [[AVAudioChannelLayout alloc] initWithLayout:mChannelLayout];
	}

	/// Returns the name of this channel layout
	///
	/// This is the value of @c kAudioFormatProperty_ChannelLayoutName or @c kAudioFormatProperty_ChannelLayoutSimpleName
	NSString * _Nullable LayoutName(bool simpleName = false) const noexcept
	{
		return (__bridge_transfer NSString *)CopyLayoutName(simpleName);
	}

	/// Returns a string representation of this channel layout
	NSString * _Nullable LayoutDescription() const noexcept
	{
		return (__bridge_transfer NSString *)CopyLayoutDescription();
	}
#endif /* __OBJC__ */

private:

	/// The underlying @c AudioChannelLayout struct
	AudioChannelLayout * _Nullable mChannelLayout = nullptr;

};

} /* namespace SFB */
