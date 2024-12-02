//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import <initializer_list>
#import <string>
#import <string_view>

#import <CoreFoundation/CoreFoundation.h>

namespace SFB {

/// Creates a @c std::string by concatenating the contents of an initializer list and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param il An initializer list containing the string views to concatenate
/// @return A @c std::string containing the concatenated result
std::string concat(std::initializer_list<std::string_view> il);

/// Creates a @c std::string from a @c CFString using UTF-8 and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param str A @c CFString containing the characters to copy
/// @return A @c std::string containing the contents of @c str in UTF-8
std::string string_from_cfstring(CFStringRef str);

/// Creates a string representation of a four-character code and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param fourcc A four-character code
/// @return A @c std::string containing a string representation of @c fourcc
std::string fourcc_string(uint32_t fourcc);

/// Creates a string representation of an @c OSStatus result code and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
/// @param code An @c OSStatus result code
/// @return A @c std::string containing a string representation of @c code
std::string osstatus_string(int32_t code);

} // namespace SFB
