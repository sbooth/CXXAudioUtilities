//
// Copyright (c) 2021 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <cctype>

#import "SFBStringFormatting.hpp"

namespace {

/// Returns true if @c i consists of four printing characters
constexpr bool fourcc_isprint(uint32_t i) noexcept
{
	return std::isprint(static_cast<unsigned char>(i)) && std::isprint(static_cast<unsigned char>(i >> 8)) && std::isprint(static_cast<unsigned char>(i >> 16)) && std::isprint(static_cast<unsigned char>(i >> 24));
}

/// Creates a @c std::string containing @c fourcc formatted as four characters and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
std::string fourcc_fourchar_string(uint32_t fourcc)
{
	return {
#if __BIG_ENDIAN__
		static_cast<char>(fourcc),
		static_cast<char>(fourcc >> 8),
		static_cast<char>(fourcc >> 16),
		static_cast<char>(fourcc >> 24),
#elif __LITTLE_ENDIAN__
		static_cast<char>(fourcc >> 24),
		static_cast<char>(fourcc >> 16),
		static_cast<char>(fourcc >> 8),
		static_cast<char>(fourcc),
#else
	#error "Unknown endianness"
#endif
	};
}

/// Creates a @c std::string containing @c val formatted as hexadecimal and returns the result
/// @throw @c std::length_error
/// @throw @c std::bad_alloc
/// @throw @c std::bad_array_new_length
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
std::string to_hex_string(T val, std::string::size_type len = sizeof(T) << 1)
{
	static const char *digits = "0123456789ABCDEF";
	std::string result(len, '0');
	for(std::string::size_type i = 0, j = (len - 1) * 4; i < len; ++i, j -= 4)
		result[i] = digits[(val >> j) & 0x0f];
	return result;
}

} // namespace

std::string SFB::concat(std::initializer_list<std::string_view> il)
{
	std::string::size_type len = 0;
	for(auto s : il)
		len += s.size();
	std::string result;
	result.reserve(len);
	for(auto s : il)
		result.append(s);
	return result;
}

std::string SFB::string_from_cfstring(CFStringRef str)
{
	if(!str)
		return {};

	auto range = CFRange{ .location = 0, .length = CFStringGetLength(str) };
	auto max_size = CFStringGetMaximumSizeForEncoding(range.length, kCFStringEncodingUTF8);

	std::string result;
	result.reserve(max_size);

	char buf [128];
	while(range.length > 0) {
		CFIndex bytesWritten = 0;
		auto converted = CFStringGetBytes(str, range, kCFStringEncodingUTF8, 0, false, reinterpret_cast<UInt8 *>(buf), sizeof buf, &bytesWritten);
		result.append(buf, static_cast<std::string::size_type>(bytesWritten));

		range.location += converted;
		range.length -= converted;
	}

	return result;
}

std::string SFB::fourcc_string(uint32_t fourcc)
{
	if(fourcc_isprint(fourcc)) {
		return concat({"'", fourcc_fourchar_string(fourcc), "'"});
	}
	else
		return concat({"0x", to_hex_string(fourcc)});
}

std::string SFB::osstatus_string(int32_t code)
{
	if(fourcc_isprint(static_cast<uint32_t>(code))) 
		return fourcc_fourchar_string(static_cast<uint32_t>(code));
	else if(code > -200000 && code < 200000)
		return std::to_string(code);
	else
		return to_hex_string(static_cast<uint32_t>(code));
}
