//
// Copyright © 2021-2024 Stephen F. Booth <me@sbooth.org>
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
	constexpr ExtAudioFileWrapper() noexcept = default;

	// This class is non-copyable
	ExtAudioFileWrapper(const ExtAudioFileWrapper&) = delete;

	// This class is non-assignable
	ExtAudioFileWrapper& operator=(const ExtAudioFileWrapper&) = delete;

	/// Calls @c ExtAudioFileDispose on the managed @c ExtAudioFile
	~ExtAudioFileWrapper()
	{
		reset();
	}

	/// Move constructor
	ExtAudioFileWrapper(ExtAudioFileWrapper&& rhs) noexcept
	: mExtAudioFile{rhs.release()}
	{}

	// Move assignment operator
	ExtAudioFileWrapper& operator=(ExtAudioFileWrapper&& rhs) noexcept
	{
		if(this != &rhs)
			reset(rhs.release());
		return *this;
	}

	/// Creates an @c ExtAudioFileWrapper managing @c extAudioFile
	ExtAudioFileWrapper(ExtAudioFileRef _Nullable extAudioFile) noexcept
	: mExtAudioFile{extAudioFile}
	{}

	/// Returns @c true if the managed @c ExtAudioFile is not @c nullptr
	explicit operator bool() const noexcept
	{
		return mExtAudioFile != nullptr;
	}

	/// Returns the managed @c ExtAudioFile
	operator ExtAudioFileRef _Nullable() const noexcept
	{
		return mExtAudioFile;
	}

	/// Returns the managed @c ExtAudioFile
	ExtAudioFileRef _Nullable get() const noexcept
	{
		return mExtAudioFile;
	}

	/// Disposes of the managed @c ExtAudioFile and replaces it with @c extAudioFile
	void reset(ExtAudioFileRef _Nullable extAudioFile = nullptr) noexcept
	{
		if(auto oldExtAudioFile = std::exchange(mExtAudioFile, extAudioFile); oldExtAudioFile)
			ExtAudioFileDispose(oldExtAudioFile);
	}

	/// Swaps the managed @c ExtAudioFile of @c *this and @c other
	void swap(ExtAudioFileWrapper& other) noexcept
	{
		std::swap(mExtAudioFile, other.mExtAudioFile);
	}

	/// Releases ownership of the managed @c ExtAudioFile and returns it
	ExtAudioFileRef _Nullable release() noexcept
	{
		return std::exchange(mExtAudioFile, nullptr);
	}

private:

	/// The managed @c ExtAudioFile
	ExtAudioFileRef _Nullable mExtAudioFile = nullptr;

};

} // namespace SFB
