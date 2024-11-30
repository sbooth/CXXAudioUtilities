//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <utility>

#import <AudioToolbox/ExtendedAudioFile.h>

namespace SFB {

/// A bare-bones wrapper around @c ExtAudioFile that does little more than call @c ExtAudioFileDispose on destruction
class ExtAudioFileWrapper
{

public:

	/// Creates an empty @c ExtAudioFileWrapper
	ExtAudioFileWrapper() noexcept = default;

	// This class is non-copyable
	ExtAudioFileWrapper(const ExtAudioFileWrapper& rhs) = delete;

	// This class is non-assignable
	ExtAudioFileWrapper& operator=(const ExtAudioFileWrapper& rhs) = delete;

	/// Calls @c ExtAudioFileDispose on the managed @c ExtAudioFile
	inline ~ExtAudioFileWrapper()
	{
		reset();
	}

	/// Move constructor
	inline ExtAudioFileWrapper(ExtAudioFileWrapper&& rhs) noexcept
	: mExtAudioFile{rhs.release()}
	{}

	// Move assignment operator
	inline ExtAudioFileWrapper& operator=(ExtAudioFileWrapper&& rhs) noexcept
	{
		if(this != &rhs)
			reset(rhs.release());
		return *this;
	}

	/// Creates an @c ExtAudioFileWrapper managing @c extAudioFile
	inline ExtAudioFileWrapper(ExtAudioFileRef extAudioFile) noexcept
	: mExtAudioFile{extAudioFile}
	{}

	/// Returns @c true if the managed @c ExtAudioFile is not @c nullptr
	inline explicit operator bool() const noexcept
	{
		return mExtAudioFile != nullptr;
	}

	/// Returns the managed @c ExtAudioFile
	inline operator ExtAudioFileRef() const noexcept
	{
		return mExtAudioFile;
	}

	/// Returns the managed @c ExtAudioFile
	inline ExtAudioFileRef get() const noexcept
	{
		return mExtAudioFile;
	}

	/// Disposes of the managed @c ExtAudioFile and replaces it with @c extAudioFile
	inline void reset(ExtAudioFileRef extAudioFile = nullptr) noexcept
	{
		auto oldExtAudioFile = mExtAudioFile;
		mExtAudioFile = extAudioFile;
		if(oldExtAudioFile)
			ExtAudioFileDispose(oldExtAudioFile);
	}

	/// Swaps the managed @c ExtAudioFile of @c *this and @c other
	inline void swap(ExtAudioFileWrapper& other) noexcept
	{
		std::swap(mExtAudioFile, other.mExtAudioFile);
	}

	/// Releases ownership of the managed @c ExtAudioFile and returns it
	inline ExtAudioFileRef release() noexcept
	{
		return std::exchange(mExtAudioFile, nullptr);
	}

private:

	/// The managed @c ExtAudioFile
	ExtAudioFileRef mExtAudioFile = nullptr;

};

} // namespace SFB
