//
// Copyright © 2020-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <os/lock.h>

namespace SFB {

/// A wrapper around @c os_unfair_lock implementing C++ @c Lockable
///
/// This class may be used with @c std::lock_guard for a scope-based lock.
///
/// @code
/// SFB::UnfairLock _lock;
/// // Later
/// std::lock_guard<SFB::UnfairLock> lock(_lock);
/// @endcode
class UnfairLock
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c UnfairLock
	constexpr UnfairLock() noexcept
	: mLock{OS_UNFAIR_LOCK_INIT}
	{}

	// This class is non-copyable
	UnfairLock(const UnfairLock&) = delete;

	// This class is non-assignable
	UnfairLock& operator=(const UnfairLock&) = delete;

	// Destructor
	~UnfairLock() = default;

	// This class is non-movable
	UnfairLock(const UnfairLock&&) = delete;

	// This class is non-move assignable
	UnfairLock& operator=(const UnfairLock&&) = delete;

#pragma mark Lockable

	/// Locks the lock
	void lock() noexcept
	{
		os_unfair_lock_lock(&mLock);
	}

	/// Unlocks the lock
	void unlock() noexcept
	{
		os_unfair_lock_unlock(&mLock);
	}

	/// Attempts to lock the lock
	/// @return @c true if the lock was successfully locked, @c false on error
	bool try_lock() noexcept
	{
		return os_unfair_lock_trylock(&mLock);
	}

#pragma mark Ownership

	/// Asserts that the calling thread is the current owner of the lock.
	///
	/// If the lock is currently owned by the calling thread, this function returns.
	///
	/// If the lock is unlocked or owned by a different thread, this function
	/// asserts and terminates the process.
	void assert_owner() noexcept
	{
		os_unfair_lock_assert_owner(&mLock);
	}

	///	Asserts that the calling thread is not the current owner of the lock.
	///
	///	If the lock is unlocked or owned by a different thread, this function returns.
	///
	///	If the lock is currently owned by the current thread, this function asserts
	///	and terminates the process.
	void assert_not_owner() noexcept
	{
		os_unfair_lock_assert_not_owner(&mLock);
	}

private:

	/// The primitive lock
	os_unfair_lock mLock;

};

} /* namespace SFB */
