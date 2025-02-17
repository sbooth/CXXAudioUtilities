//
// Copyright © 2010-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <stdexcept>

#import <dispatch/dispatch.h>

namespace SFB {

/// A wrapper around a @c dispatch_semaphore_t counting semaphore.
class DispatchSemaphore
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c DispatchSemaphore
	/// @param value The starting value for the semaphore.
	/// @throw @c std::runtime_error If the semaphore could not be created
	DispatchSemaphore(intptr_t value)
	{
		mSemaphore = dispatch_semaphore_create(value);
		if(!mSemaphore)
			throw std::runtime_error("Unable to create dispatch semaphore");
	}

	// This class is non-copyable
	DispatchSemaphore(const DispatchSemaphore&) = delete;

	// This class is non-assignable
	DispatchSemaphore& operator=(const DispatchSemaphore&) = delete;

	// Destructor
	~DispatchSemaphore()
	{
#if !__has_feature(objc_arc)
		dispatch_release(mSemaphore);
#endif /* !__has_feature(objc_arc) */
	}

	// This class is non-movable
	DispatchSemaphore(const DispatchSemaphore&&) = delete;

	// This class is non-move assignable
	DispatchSemaphore& operator=(const DispatchSemaphore&&) = delete;

#pragma mark Signaling and Waiting

	 /// Signals (increments) the semaphore.
	 /// If the previous value was less than zero, this function wakes a waiting thread.
	 /// @return \c true if a thread was woken, \c false otherwise
	bool Signal() noexcept
	{
		return dispatch_semaphore_signal(mSemaphore) != 0;
	}

	 /// Waits for (decrements) the semaphore.
	 /// If the resulting value is less than zero this function waits for a signal to occur before returning.
	 /// @param duration The maximum duration to block
	 /// @return \c true if successful, \c false if the timeout occurred
	bool Wait(dispatch_time_t duration = DISPATCH_TIME_FOREVER) noexcept
	{
		return dispatch_semaphore_wait(mSemaphore, duration) == 0;
	}

private:

	/// The libdispatch semaphore
	dispatch_semaphore_t _Nonnull mSemaphore;

};

} /* namespace SFB */
