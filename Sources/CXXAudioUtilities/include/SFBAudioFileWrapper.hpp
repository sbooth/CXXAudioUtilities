//
// Copyright © 2021-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <utility>

#import <AudioToolbox/AudioFile.h>

namespace SFB {

/// A bare-bones wrapper around @c AudioFile that does little more than call @c AudioFileClose on destruction
class AudioFileWrapper
{

public:

	/// Creates an empty @c AudioFileWrapper
	constexpr AudioFileWrapper() noexcept = default;

	// This class is non-copyable
	AudioFileWrapper(const AudioFileWrapper&) = delete;

	// This class is non-assignable
	AudioFileWrapper& operator=(const AudioFileWrapper&) = delete;

	/// Calls @c AudioFileClose on the managed @c AudioFile
	~AudioFileWrapper()
	{
		reset();
	}

	/// Move constructor
	AudioFileWrapper(AudioFileWrapper&& rhs) noexcept
	: mAudioFile{rhs.release()}
	{}

	// Move assignment operator
	AudioFileWrapper& operator=(AudioFileWrapper&& rhs) noexcept
	{
		if(this != &rhs)
			reset(rhs.release());
		return *this;
	}

	/// Creates an @c AudioFileWrapper managing @c audioFile
	AudioFileWrapper(AudioFileID _Nullable audioFile) noexcept
	: mAudioFile{audioFile}
	{}

	/// Returns @c true if the managed @c AudioFile is not @c nullptr
	explicit operator bool() const noexcept
	{
		return mAudioFile != nullptr;
	}

	/// Returns the managed @c AudioFile
	operator AudioFileID _Nullable() const noexcept
	{
		return mAudioFile;
	}

	/// Returns the managed @c AudioFile
	AudioFileID _Nullable get() const noexcept
	{
		return mAudioFile;
	}

	/// Closes the managed @c AudioFile and replaces it with @c audioFile
	void reset(AudioFileID _Nullable audioFile = nullptr) noexcept
	{
		if(auto old = std::exchange(mAudioFile, audioFile); old)
			AudioFileClose(old);
	}

	/// Swaps the managed @c AudioFile of @c *this and @c other
	void swap(AudioFileWrapper& other) noexcept
	{
		std::swap(mAudioFile, other.mAudioFile);
	}

	/// Releases ownership of the managed @c AudioFile and returns it
	AudioFileID _Nullable release() noexcept
	{
		return std::exchange(mAudioFile, nullptr);
	}

private:

	/// The managed @c AudioFile
	AudioFileID _Nullable mAudioFile = nullptr;

};

} /* namespace SFB */
