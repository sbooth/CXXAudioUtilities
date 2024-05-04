//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <functional>

namespace SFB {

/// A class that calls a function upon destruction.
///
/// This allows similar functionality to @c defer in Swift and Go.
/// This is useful to limit the lifetime of non-C++ objects and provides an alternative to @c std::unique_ptr with a
/// custom deleter.
///
/// @code
/// ExtAudioFileRef eaf;
/// auto result = ExtAudioFileOpenURL(url, &eaf);
/// assert(result == noErr);
/// auto lambda = [eaf]() {
///     auto result = ExtAudioFileDispose(eaf);
///     assert(result == noErr);
/// };
/// SFB::ScopeGuard cleanup(lambda);
/// @endcode
class ScopeGuard
{

public:

	/// Creates a new @c ScopeGuard executing @c cleanup when the destructor is called
	template <typename F>
	inline ScopeGuard(const F& cleanup) noexcept
	: mCleanup{cleanup}
	{}

	// This class is non-copyable
	ScopeGuard(const ScopeGuard& rhs) = delete;

	// This class is non-assignable
	ScopeGuard& operator=(const ScopeGuard& rhs) = delete;

	/// Executes the cleanup function
	inline ~ScopeGuard()
	{
		mCleanup();
	}

	// This class is non-movable
	ScopeGuard(const ScopeGuard&& rhs) = delete;

	// This class is non-move assignable
	ScopeGuard& operator=(const ScopeGuard&& rhs) = delete;

private:

	/// The function to invoke upon object destruction
	std::function<void()> mCleanup;

};

} // namespace SFB
