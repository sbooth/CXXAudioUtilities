//
// Copyright © 2020-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <algorithm>
#import <optional>
#import <stdexcept>
#import <type_traits>

#import <libkern/OSByteOrder.h>

namespace SFB {

/// A @c ByteStream provides heterogeneous typed access to an untyped buffer.
class ByteStream
{

public:

	/// Creates an empty @c ByteStream
	constexpr ByteStream() noexcept = default;

	/// Initializes a @c ByteStream object with the same buffer, length, and read position as @c rhs
	/// @param rhs The object to copy
	ByteStream(const ByteStream& rhs) noexcept
	: mBuffer{rhs.mBuffer}, mBufferLength{rhs.mBufferLength}, mReadPosition{rhs.mReadPosition}
	{}

	/// Sets the buffer, length, and read position to those of @c rhs
	/// @param rhs The object to copy
	/// @return A reference to @c this
	ByteStream& operator=(const ByteStream& rhs) noexcept
	{
		if(this != &rhs) {
			mBuffer = rhs.mBuffer;
			mBufferLength = rhs.mBufferLength;
			mReadPosition = rhs.mReadPosition;
		}
		return *this;
	}

	/// Destructor
	~ByteStream() = default;

	/// Move constructor
	ByteStream(ByteStream&& rhs) noexcept
	: mBuffer{rhs.mBuffer}, mBufferLength{rhs.mBufferLength}, mReadPosition{rhs.mReadPosition}
	{
		rhs.mBuffer = nullptr;
		rhs.mBufferLength = 0;
		rhs.mReadPosition = 0;
	}

	/// Move assignment operator
	ByteStream& operator=(ByteStream&& rhs) noexcept
	{
		if(this != &rhs) {
			mBuffer = rhs.mBuffer;
			mBufferLength = rhs.mBufferLength;
			mReadPosition = rhs.mReadPosition;

			rhs.mBuffer = nullptr;
			rhs.mBufferLength = 0;
			rhs.mReadPosition = 0;
		}
		return *this;
	}


	/// Initializes a @c ByteStream object with the specified buffer and length and sets the read position to @c 0
	/// @param buf The buffer providing the data
	/// @param len The length of @c buf in bytes
	/// @throw @c std::invalid_argument if @c buf==nullptr and @c len>0
	ByteStream(const void * const _Nullable buf, size_t len)
	: mBuffer{buf}, mBufferLength{len}
	{
		if(!mBuffer && len > 0)
			throw std::invalid_argument("!mBuffer && len > 0");
	}


	/// Compares to @c ByteStream objects for equality.
	/// Two @c ByteStream objects are equal if they have the same buffer, length, and read position
	/// @param rhs The object to compare
	/// @return @c true if the objects are equal, @c false otherwise
	bool operator==(const ByteStream& rhs) noexcept
	{
		return mBuffer == rhs.mBuffer && mBufferLength == rhs.mBufferLength && mReadPosition == rhs.mReadPosition;
	}

	/// Compares to @c ByteStream objects for inequality.
	/// Two @c ByteStream objects are equal if they have the same buffer, length, and read position
	/// @param rhs The object to compare
	/// @return @c true if the objects are not equal, @c false otherwise
	bool operator!=(const ByteStream& rhs) noexcept
	{
		return mBuffer != rhs.mBuffer || mBufferLength != rhs.mBufferLength || mReadPosition != rhs.mReadPosition;
	}


	/// Reads a value and advances the read position.
	/// @tparam T The type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
	bool Read(T& value) noexcept
	{
		auto size = sizeof(T);
		if(size > Remaining())
			return false;
		auto bytesRead = Read(static_cast<void *>(&value), size);
		return bytesRead == size;
	}

	/// Reads a value and advances the read position.
	/// @tparam T The type to read
	/// @return The value read or @c std::nullopt on failure
	template <typename T, typename = std::enable_if_t<std::is_trivially_default_constructible_v<T>>>
	std::optional<T> Read() noexcept
	{
		T value{};
		if(!Read(value))
			return std::nullopt;
		return value;
	}

	/// Reads a little endian value, converts it to host byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
	bool ReadLE(T& value) noexcept
	{
		if(!Read(value))
			return false;

		switch(sizeof(T)) {
			case 2:	value = static_cast<T>(OSSwapLittleToHostInt16(value)); break;
			case 4:	value = static_cast<T>(OSSwapLittleToHostInt32(value)); break;
			case 8:	value = static_cast<T>(OSSwapLittleToHostInt64(value)); break;
		}

		return true;
	}

	/// Reads a little endian value, converts it to host byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @return The value read or @c std::nullopt on failure
	template <typename T, typename = std::enable_if_t<std::is_trivially_default_constructible_v<T>>>
	std::optional<T> ReadLE() noexcept
	{
		T value{};
		if(!ReadLE(value))
			return std::nullopt;
		return value;
	}

	/// Reads a big endian value, converts it to host byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
	bool ReadBE(T& value) noexcept
	{
		if(!Read(value))
			return false;

		switch(sizeof(T)) {
			case 2:	value = static_cast<T>(OSSwapBigToHostInt16(value)); break;
			case 4:	value = static_cast<T>(OSSwapBigToHostInt32(value)); break;
			case 8:	value = static_cast<T>(OSSwapBigToHostInt64(value)); break;
		}

		return true;
	}

	/// Reads a big endian value, converts it to host byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @return The value read or @c std::nullopt on failure
	template <typename T, typename = std::enable_if_t<std::is_trivially_default_constructible_v<T>>>
	std::optional<T> ReadBE() noexcept
	{
		T value{};
		if(!ReadBE(value))
			return std::nullopt;
		return value;
	}

	/// Reads a value, swaps its byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @param value The destination value
	/// @return @c true on success, @c false otherwise
	template <typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
	bool ReadSwapped(T& value) noexcept
	{
		if(!Read(value))
			return false;

		switch(sizeof(T)) {
			case 2: value = static_cast<T>(OSSwapInt16(value)); break;
			case 4: value = static_cast<T>(OSSwapInt32(value)); break;
			case 8: value = static_cast<T>(OSSwapInt64(value)); break;
		}

		return true;
	}

	/// Reads a value, swaps its byte ordering, and advances the read position.
	/// @tparam T The type to read
	/// @return The value read or @c std::nullopt on failure
	template <typename T, typename = std::enable_if_t<std::is_trivially_default_constructible_v<T>>>
	std::optional<T> ReadSwapped() noexcept
	{
		T value{};
		if(!ReadSwapped(value))
			return std::nullopt;
		return value;
	}


	/// Reads bytes and advances the read position.
	/// @param buf The destination buffer or @c nullptr to discard the bytes
	/// @param count The number of bytes to read
	/// @return The number of bytes actually read
	size_t Read(void * const _Nullable buf, size_t count) noexcept
	{
		auto bytesToCopy = std::min(count, mBufferLength - mReadPosition);
		if(buf)
			std::memcpy(buf, static_cast<const uint8_t *>(mBuffer) + mReadPosition, bytesToCopy);
		mReadPosition += bytesToCopy;
		return bytesToCopy;
	}

	/// Advances the read position.
	/// @param count The number of bytes to skip
	/// @return The number of bytes actually skipped
	size_t Skip(size_t count) noexcept
	{
		auto bytesToSkip = std::min(count, mBufferLength - mReadPosition);
		mReadPosition += bytesToSkip;
		return bytesToSkip;
	}

	/// Rewinds the read position.
	/// @param count The number of bytes to rewind
	/// @return The number of bytes actually skipped
	size_t Rewind(size_t count) noexcept
	{
		auto bytesToSkip = std::min(count, mReadPosition);
		mReadPosition -= bytesToSkip;
		return bytesToSkip;
	}

	/// Returns the number of bytes in the buffer.
	/// @return The number of bytes in the buffer
	inline constexpr size_t Length() const noexcept
	{
		return mBufferLength;
	}

	/// Returns the number of bytes remaining.
	inline constexpr size_t Remaining() const noexcept
	{
		return mBufferLength - mReadPosition;
	}

	/// Returns the read position.
	/// @return The read posiiton
	inline constexpr size_t Position() const noexcept
	{
		return mReadPosition;
	}

	/// Sets the read position.
	/// @param pos The desired read position
	/// @return The new read posiiton
	inline size_t SetPosition(size_t pos) noexcept
	{
		mReadPosition = std::min(pos, mBufferLength);
		return mReadPosition;
	}

private:

	/// The wrapped buffer
	const void * _Nullable mBuffer = nullptr;
	/// The number of bytes in @c mBuffer
	size_t mBufferLength = 0;
	/// The current read position
	size_t mReadPosition = 0;

};

} // namespace SFB
