//
// Copyright © 2013-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <cstdlib>
#import <cstring>
#import <new>

#import <AudioToolbox/AudioFormat.h>

#import "SFBCAChannelLayout.hpp"

namespace {

/// Returns the size in bytes of an @c AudioChannelLayout with the specified number of channel descriptions
constexpr size_t ChannelLayoutSize(UInt32 numberChannelDescriptions) noexcept
{
	return offsetof(AudioChannelLayout, mChannelDescriptions) + (numberChannelDescriptions * sizeof(AudioChannelDescription));
}

/// Allocates an @c AudioChannelLayout
/// @param numberChannelDescriptions The number of channel descriptions that will be stored in the channel layout
/// @return An @c AudioChannelLayout
/// @throws @c std::bad_alloc
AudioChannelLayout * CreateChannelLayout(UInt32 numberChannelDescriptions)
{
	size_t layoutSize = ChannelLayoutSize(numberChannelDescriptions);
	AudioChannelLayout *channelLayout = static_cast<AudioChannelLayout *>(std::malloc(layoutSize));
	if(!channelLayout)
		throw std::bad_alloc();

	std::memset(channelLayout, 0, layoutSize);

	channelLayout->mNumberChannelDescriptions = numberChannelDescriptions;

	return channelLayout;
}

/// Creates and returns a copy of @c rhs
/// @param rhs The @c AudioChannelLayout to copy
/// @return An @c AudioChannelLayout
/// @throws @c std::bad_alloc
AudioChannelLayout * CopyChannelLayout(const AudioChannelLayout * _Nullable rhs)
{
	if(!rhs)
		return nullptr;

	size_t layoutSize = ChannelLayoutSize(rhs->mNumberChannelDescriptions);
	AudioChannelLayout *channelLayout = static_cast<AudioChannelLayout *>(std::malloc(layoutSize));
	if(!channelLayout)
		throw std::bad_alloc();

	std::memcpy(channelLayout, rhs, layoutSize);

	return channelLayout;
}

/// Returns the string representation of an @c AudioChannelLayoutTag
constexpr const char * GetChannelLayoutTagName(AudioChannelLayoutTag layoutTag) noexcept
{
	switch(layoutTag) {
		case kAudioChannelLayoutTag_UseChannelDescriptions:
			return "Use Channel Descriptions";
		case kAudioChannelLayoutTag_UseChannelBitmap:
			return "Use Channel Bitmap";

		case kAudioChannelLayoutTag_Mono:					return "Mono";
		case kAudioChannelLayoutTag_Stereo:					return "Stereo";
		case kAudioChannelLayoutTag_StereoHeadphones:		return "Stereo Headphones";
		case kAudioChannelLayoutTag_MatrixStereo:			return "Matrix Stereo";
		case kAudioChannelLayoutTag_MidSide:				return "Mid-Side";
		case kAudioChannelLayoutTag_XY:						return "XY";
		case kAudioChannelLayoutTag_Binaural:				return "Binaural";
		case kAudioChannelLayoutTag_Ambisonic_B_Format:		return "Ambisonic B-format";
		case kAudioChannelLayoutTag_Quadraphonic:			return "Quadraphonic";
		case kAudioChannelLayoutTag_Pentagonal:				return "Pentagonal";
		case kAudioChannelLayoutTag_Hexagonal:				return "Hexagonal";
		case kAudioChannelLayoutTag_Octagonal:				return "Octagonal";
		case kAudioChannelLayoutTag_Cube:					return "Cube";
		case kAudioChannelLayoutTag_MPEG_3_0_A:				return "MPEG 3.0 A";
		case kAudioChannelLayoutTag_MPEG_3_0_B:				return "MPEG 3.0 B";
		case kAudioChannelLayoutTag_MPEG_4_0_A:				return "MPEG 4.0 A";
		case kAudioChannelLayoutTag_MPEG_4_0_B:				return "MPEG 4.0 B";
		case kAudioChannelLayoutTag_MPEG_5_0_A:				return "MPEG 5.0 A";
		case kAudioChannelLayoutTag_MPEG_5_0_B:				return "MPEG 5.0 B";
		case kAudioChannelLayoutTag_MPEG_5_0_C:				return "MPEG 5.0 C";
		case kAudioChannelLayoutTag_MPEG_5_0_D:				return "MPEG 5.0 D";
		case kAudioChannelLayoutTag_MPEG_5_1_A:				return "MPEG 5.1 A";
		case kAudioChannelLayoutTag_MPEG_5_1_B:				return "MPEG 5.1 B";
		case kAudioChannelLayoutTag_MPEG_5_1_C:				return "MPEG 5.1 C";
		case kAudioChannelLayoutTag_MPEG_5_1_D:				return "MPEG 5.1 D";
		case kAudioChannelLayoutTag_MPEG_6_1_A:				return "MPEG 6.1 A";
		case kAudioChannelLayoutTag_MPEG_7_1_A:				return "MPEG 7.1 A";
		case kAudioChannelLayoutTag_MPEG_7_1_B:				return "MPEG 7.1 B";
		case kAudioChannelLayoutTag_MPEG_7_1_C:				return "MPEG 7.1 C";
		case kAudioChannelLayoutTag_Emagic_Default_7_1:		return "Emagic Default 7.1";
		case kAudioChannelLayoutTag_SMPTE_DTV:				return "SMPTE DTV";
		case kAudioChannelLayoutTag_ITU_2_1:				return "ITU 2.1";
		case kAudioChannelLayoutTag_ITU_2_2:				return "ITU 2.2";
		case kAudioChannelLayoutTag_DVD_4:					return "DVD 4";
		case kAudioChannelLayoutTag_DVD_5:					return "DVD 5";
		case kAudioChannelLayoutTag_DVD_6:					return "DVD 6";
		case kAudioChannelLayoutTag_DVD_10:					return "DVD 10";
		case kAudioChannelLayoutTag_DVD_11:					return "DVD 11";
		case kAudioChannelLayoutTag_DVD_18:					return "DVD 18";
		case kAudioChannelLayoutTag_AudioUnit_6_0:			return "AudioUnit 6.0";
		case kAudioChannelLayoutTag_AudioUnit_7_0:			return "AudioUnit 7.0";
		case kAudioChannelLayoutTag_AudioUnit_7_0_Front:	return "AudioUnit 7.0 Front";
		case kAudioChannelLayoutTag_AAC_6_0:				return "AAC 6.0";
		case kAudioChannelLayoutTag_AAC_6_1:				return "AAC 6.1";
		case kAudioChannelLayoutTag_AAC_7_0:				return "AAC 7.0";
		case kAudioChannelLayoutTag_AAC_7_1_B:				return "AAC 7.1 B";
		case kAudioChannelLayoutTag_AAC_7_1_C:				return "AAC 7.1 C";
		case kAudioChannelLayoutTag_AAC_Octagonal:			return "AAC Octagonal";
		case kAudioChannelLayoutTag_TMH_10_2_std:			return "TMH 10.2 standard";
		case kAudioChannelLayoutTag_TMH_10_2_full:			return "TMH 10.2 full";
		case kAudioChannelLayoutTag_AC3_1_0_1:				return "AC-3 1.0.1";
		case kAudioChannelLayoutTag_AC3_3_0:				return "AC-3 3.0";
		case kAudioChannelLayoutTag_AC3_3_1:				return "AC-3 3.1";
		case kAudioChannelLayoutTag_AC3_3_0_1:				return "AC-3 3.0.1";
		case kAudioChannelLayoutTag_AC3_2_1_1:				return "AC-3 2.1.1";
		case kAudioChannelLayoutTag_AC3_3_1_1:				return "AC-3 3.1.1";
		case kAudioChannelLayoutTag_EAC_6_0_A:				return "EAC 6.0 A";
		case kAudioChannelLayoutTag_EAC_7_0_A:				return "EAC 7.0 A";
		case kAudioChannelLayoutTag_EAC3_6_1_A:				return "E-AC-3 6.1 A";
		case kAudioChannelLayoutTag_EAC3_6_1_B:				return "E-AC-3 6.1 B";
		case kAudioChannelLayoutTag_EAC3_6_1_C:				return "E-AC-3 6.1 C";
		case kAudioChannelLayoutTag_EAC3_7_1_A:				return "E-AC-3 7.1 A";
		case kAudioChannelLayoutTag_EAC3_7_1_B:				return "E-AC-3 7.1 B";
		case kAudioChannelLayoutTag_EAC3_7_1_C:				return "E-AC-3 7.1 C";
		case kAudioChannelLayoutTag_EAC3_7_1_D:				return "E-AC-3 7.1 D";
		case kAudioChannelLayoutTag_EAC3_7_1_E:				return "E-AC-3 7.1 E";
		case kAudioChannelLayoutTag_EAC3_7_1_F:				return "E-AC-3 7.1 F";
		case kAudioChannelLayoutTag_EAC3_7_1_G:				return "E-AC-3 7.1 G";
		case kAudioChannelLayoutTag_EAC3_7_1_H:				return "E-AC-3 7.1 H";
		case kAudioChannelLayoutTag_DTS_3_1:				return "DTS 3.1";
		case kAudioChannelLayoutTag_DTS_4_1:				return "DTS 4.1";
		case kAudioChannelLayoutTag_DTS_6_0_A:				return "DTS 6.0 A";
		case kAudioChannelLayoutTag_DTS_6_0_B:				return "DTS 6.0 B";
		case kAudioChannelLayoutTag_DTS_6_0_C:				return "DTS 6.0 C";
		case kAudioChannelLayoutTag_DTS_6_1_A:				return "DTS 6.1 A";
		case kAudioChannelLayoutTag_DTS_6_1_B:				return "DTS 6.1 B";
		case kAudioChannelLayoutTag_DTS_6_1_C:				return "DTS 6.1 C";
		case kAudioChannelLayoutTag_DTS_7_0:				return "DTS 7.0";
		case kAudioChannelLayoutTag_DTS_7_1:				return "DTS 7.1";
		case kAudioChannelLayoutTag_DTS_8_0_A:				return "DTS 8.0 A";
		case kAudioChannelLayoutTag_DTS_8_0_B:				return "DTS 8.0 B";
		case kAudioChannelLayoutTag_DTS_8_1_A:				return "DTS 8.1 A";
		case kAudioChannelLayoutTag_DTS_8_1_B:				return "DTS 8.1 B";
		case kAudioChannelLayoutTag_DTS_6_1_D:				return "DTS 6.1 D";
		case kAudioChannelLayoutTag_WAVE_4_0_B:				return "WAVE 4.0 B";
		case kAudioChannelLayoutTag_WAVE_5_0_B:				return "WAVE 5.0 B";
		case kAudioChannelLayoutTag_WAVE_5_1_B:				return "WAVE 5.1 B";
		case kAudioChannelLayoutTag_WAVE_6_1:				return "WAVE 6.1";
		case kAudioChannelLayoutTag_WAVE_7_1:				return "WAVE 7.1";
		case kAudioChannelLayoutTag_Atmos_5_1_2:			return "Atmos 5.1.2";
		case kAudioChannelLayoutTag_Atmos_5_1_4:			return "Atmos 5.1.4";
		case kAudioChannelLayoutTag_Atmos_7_1_2:			return "Atmos 7.1.2";
		case kAudioChannelLayoutTag_Atmos_7_1_4:			return "Atmos 7.1.4";
		case kAudioChannelLayoutTag_Atmos_9_1_6:			return "Atmos 9.1.6";
		case kAudioChannelLayoutTag_Logic_4_0_C:			return "Logic 4.0 C";
		case kAudioChannelLayoutTag_Logic_6_0_B:			return "Logic 6.0 B";
		case kAudioChannelLayoutTag_Logic_6_1_B:			return "Logic 6.1 B";
		case kAudioChannelLayoutTag_Logic_6_1_D:			return "Logic 6.1 D";
		case kAudioChannelLayoutTag_Logic_7_1_B:			return "Logic 7.1 B";
		case kAudioChannelLayoutTag_Logic_Atmos_7_1_4_B:	return "Logic Atmos 7.1.4 B";
		case kAudioChannelLayoutTag_Logic_Atmos_7_1_6:		return "Logic Atmos 7.1.6";
		case kAudioChannelLayoutTag_CICP_13:				return "CICP 13";
		case kAudioChannelLayoutTag_CICP_14:				return "CICP 14";
		case kAudioChannelLayoutTag_CICP_15:				return "CICP 15";
		case kAudioChannelLayoutTag_CICP_16:				return "CICP 16";
		case kAudioChannelLayoutTag_CICP_17:				return "CICP 17";
		case kAudioChannelLayoutTag_CICP_18:				return "CICP 18";
		case kAudioChannelLayoutTag_CICP_19:				return "CICP 19";
		case kAudioChannelLayoutTag_CICP_20:				return "CICP 20";
		case kAudioChannelLayoutTag_Ogg_5_0:				return "Ogg 5.0";
		case kAudioChannelLayoutTag_Ogg_5_1:				return "Ogg 5.1";
		case kAudioChannelLayoutTag_Ogg_6_1:				return "Ogg 6.1";
		case kAudioChannelLayoutTag_Ogg_7_1:				return "Ogg 7.1";
		case kAudioChannelLayoutTag_MPEG_5_0_E:				return "MPEG 5.0 E";
		case kAudioChannelLayoutTag_MPEG_5_1_E:				return "MPEG 5.1 E";
		case kAudioChannelLayoutTag_MPEG_6_1_B:				return "MPEG 6.1 B";
		case kAudioChannelLayoutTag_MPEG_7_1_D:				return "MPEG 7.1 D";

		case kAudioChannelLayoutTag_BeginReserved...kAudioChannelLayoutTag_EndReserved:
			return "Reserved";

		default:
			break;
	}

	switch(layoutTag & 0xFFFF0000) {
		case kAudioChannelLayoutTag_HOA_ACN_SN3D:			return "HOA ACN SN3D";
		case kAudioChannelLayoutTag_HOA_ACN_N3D:			return "HOA ACN N3D";
		case kAudioChannelLayoutTag_DiscreteInOrder:		return "Discrete in Order";
		case kAudioChannelLayoutTag_Unknown:				return "Unknown";

		default:
			break;
	}

	return nullptr;
}

/// A @c std::unique_ptr deleter for @c CFTypeRef objects
struct cf_type_ref_deleter {
	void operator()(CFTypeRef cf) { CFRelease(cf); }
};

/// A @c std::unique_ptr holding a @c CFTypeRef object
template <typename T>
using cf_type_ref_unique_ptr = std::unique_ptr<std::remove_pointer_t<T>, cf_type_ref_deleter>;

/// A @c std::unique_ptr holding a @c CFArrayRef
using cf_array_unique_ptr = cf_type_ref_unique_ptr<CFArrayRef>;
/// A @c std::unique_ptr holding a @c CFStringRef
using cf_string_unique_ptr = cf_type_ref_unique_ptr<CFStringRef>;

/// Returns the name of the channel for @c channelLabel
cf_string_unique_ptr CopyChannelLabelName(AudioChannelLabel channelLabel, bool shortName) noexcept
{
	const auto property = shortName ? kAudioFormatProperty_ChannelShortName : kAudioFormatProperty_ChannelName;

	CFStringRef channelName = nullptr;
	UInt32 dataSize = sizeof(channelName);
	OSStatus result = AudioFormatGetProperty(property, sizeof(channelLabel), &channelLabel, &dataSize, &channelName);
	if(result != noErr)
		return nullptr;

	return cf_string_unique_ptr{channelName};
}

/// Returns an array of channel names for @c channelDescriptions
cf_array_unique_ptr CreateChannelNameArrayForChannelDescriptions(const AudioChannelDescription *channelDescriptions, UInt32 count, bool shortNames) noexcept
{
	if(!channelDescriptions || count == 0)
		return nullptr;

	cf_type_ref_unique_ptr<CFMutableArrayRef> array{CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks)};

	const auto property = shortNames ? kAudioFormatProperty_ChannelShortName : kAudioFormatProperty_ChannelName;

	for(UInt32 i = 0; i < count; ++i) {
		if(auto channelName = CopyChannelLabelName(channelDescriptions[i].mChannelLabel, shortNames); channelName)
			CFArrayAppendValue(array.get(), reinterpret_cast<const void *>(channelName.get()));
		else
			CFArrayAppendValue(array.get(), CFSTR("?"));
	}

	return array;
}

/// Joins strings from @c array separated by @c delimiter
cf_string_unique_ptr JoinStringArray(CFArrayRef array, CFStringRef delimiter) noexcept
{
	if(!array)
		return nullptr;

	cf_type_ref_unique_ptr<CFMutableStringRef> result{CFStringCreateMutable(kCFAllocatorDefault, 0)};

	const CFIndex count = CFArrayGetCount(array);
	for(CFIndex i = 0; i < count; ++i) {
		auto value = CFArrayGetValueAtIndex(array, i);
		// Skip non-strings
		if(CFGetTypeID(value) != CFStringGetTypeID())
			continue;
		CFStringAppend(result.get(), static_cast<CFStringRef>(value));
		if(delimiter && i < count - 1)
			CFStringAppend(result.get(), delimiter);
	}

	return result;
}

} // namespace

size_t SFB::AudioChannelLayoutSize(const AudioChannelLayout *channelLayout) noexcept
{
	if(!channelLayout)
		return 0;
	return ChannelLayoutSize(channelLayout->mNumberChannelDescriptions);
}

CFStringRef SFB::CopyAudioChannelLayoutName(const AudioChannelLayout * channelLayout, bool simpleName) noexcept
{
	if(!channelLayout)
		return nullptr;

	CFStringRef name = nullptr;
	UInt32 dataSize = sizeof(name);
	OSStatus result = AudioFormatGetProperty(simpleName ? kAudioFormatProperty_ChannelLayoutSimpleName : kAudioFormatProperty_ChannelLayoutName, static_cast<UInt32>(AudioChannelLayoutSize(channelLayout)), channelLayout, &dataSize, &name);
	if(result == noErr)
		return name;
	else
		return nullptr;
}

CFStringRef SFB::CopyAudioChannelLayoutDescription(const AudioChannelLayout *channelLayout) noexcept
{
	if(!channelLayout)
		return nullptr;

	cf_string_unique_ptr layoutName{CopyAudioChannelLayoutName(channelLayout)};

	if(channelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelDescriptions){
		// kAudioFormatProperty_ChannelLayoutName returns '!fmt' for kAudioChannelLabel_UseCoordinates
		if(layoutName)
			return CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("Channel Descriptions: %u ch, %@"), channelLayout->mNumberChannelDescriptions, layoutName.get());

		CFMutableStringRef result = CFStringCreateMutable(kCFAllocatorDefault, 0);
		if(!result)
			return nullptr;

		CFStringAppendFormat(result, nullptr, CFSTR("Channel Descriptions: %u ch"), channelLayout->mNumberChannelDescriptions);

		cf_type_ref_unique_ptr<CFMutableArrayRef> array{CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks)};

		const AudioChannelDescription *desc = channelLayout->mChannelDescriptions;
		for(UInt32 i = 0; i < channelLayout->mNumberChannelDescriptions; ++i, ++desc) {
			if(desc->mChannelLabel == kAudioChannelLabel_UseCoordinates) {
				cf_string_unique_ptr coordinateString{};
				if(desc->mChannelFlags & kAudioChannelFlags_RectangularCoordinates)
					coordinateString.reset(CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("[x: %g, y: %g, z: %g%s]"), desc->mCoordinates[0], desc->mCoordinates[1], desc->mCoordinates[2], desc->mChannelFlags & kAudioChannelFlags_Meters ? " m" : ""));
				else if(desc->mChannelFlags & kAudioChannelFlags_SphericalCoordinates)
					coordinateString.reset(CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("[r: %g, θ: %g, φ: %g%s]"), desc->mCoordinates[2], desc->mCoordinates[1], desc->mCoordinates[0], desc->mChannelFlags & kAudioChannelFlags_Meters ? " m" : ""));
				else
					coordinateString.reset(CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("[?! %g, %g, %g%s]"), desc->mCoordinates[0], desc->mCoordinates[1], desc->mCoordinates[2], desc->mChannelFlags & kAudioChannelFlags_Meters ? " m" : ""));

				CFArrayAppendValue(array.get(), reinterpret_cast<const void *>(coordinateString.get()));
			}
			else {
				if(auto channelName = CopyChannelLabelName(desc->mChannelLabel, true); channelName)
					CFArrayAppendValue(array.get(), reinterpret_cast<const void *>(channelName.get()));
				else
					CFArrayAppendValue(array.get(), CFSTR("?"));
			}
		}

		auto channelNamesString = JoinStringArray(array.get(), CFSTR(" "));
		CFStringAppendFormat(result, nullptr, CFSTR(", (%@)"), channelNamesString.get());

		return result;
	}
	else if(channelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelBitmap)
		return CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("Channel Bitmap: 0x%x (%u ch), %@"), channelLayout->mChannelBitmap, __builtin_popcount(channelLayout->mChannelBitmap), layoutName.get());
	else
		return CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("Tag: %s (0x%x, %u ch), %@"), GetChannelLayoutTagName(channelLayout->mChannelLayoutTag), channelLayout->mChannelLayoutTag, channelLayout->mChannelLayoutTag & 0xffff, layoutName.get());
}

// Constants
const SFB::CAChannelLayout SFB::CAChannelLayout::Mono		= CAChannelLayout(kAudioChannelLayoutTag_Mono);
const SFB::CAChannelLayout SFB::CAChannelLayout::Stereo 	= CAChannelLayout(kAudioChannelLayoutTag_Stereo);

SFB::CAChannelLayout SFB::CAChannelLayout::ChannelLayoutWithBitmap(AudioChannelBitmap channelBitmap)
{
	CAChannelLayout channelLayout{kAudioChannelLayoutTag_UseChannelBitmap};
	channelLayout.mChannelLayout->mChannelBitmap = channelBitmap;

	// Attempt to convert the bitmap to a layout tag
	AudioChannelLayoutTag tag;
	UInt32 dataSize = sizeof(tag);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_TagForChannelLayout, static_cast<UInt32>(AudioChannelLayoutSize(channelLayout.mChannelLayout)), channelLayout.mChannelLayout, &dataSize, &tag);
	if(result == noErr) {
		channelLayout.mChannelLayout->mChannelLayoutTag = tag;
		channelLayout.mChannelLayout->mChannelBitmap = 0;
	}

	return channelLayout;
}

SFB::CAChannelLayout SFB::CAChannelLayout::ChannelLayoutWithTag(AudioChannelLayoutTag layoutTag)
{
	return CAChannelLayout{ layoutTag };
}

SFB::CAChannelLayout SFB::CAChannelLayout::ChannelLayoutWithChannelLabels(std::vector<AudioChannelLabel> channelLabels)
{
	return CAChannelLayout{ channelLabels };
}

SFB::CAChannelLayout::CAChannelLayout(const CAChannelLayout& rhs)
: CAChannelLayout{}
{
	*this = rhs;
}

SFB::CAChannelLayout& SFB::CAChannelLayout::operator=(const CAChannelLayout& rhs)
{
	if(this != &rhs)
		Reset(CopyChannelLayout(rhs.mChannelLayout));
	return *this;
}

SFB::CAChannelLayout::CAChannelLayout(AudioChannelLayoutTag layoutTag)
: mChannelLayout{CreateChannelLayout(0)}
{
	mChannelLayout->mChannelLayoutTag = layoutTag;
}

SFB::CAChannelLayout::CAChannelLayout(std::vector<AudioChannelLabel> channelLabels)
: mChannelLayout{CreateChannelLayout(static_cast<UInt32>(channelLabels.size()))}
{
	mChannelLayout->mChannelLayoutTag = kAudioChannelLayoutTag_UseChannelDescriptions;
	for(std::vector<AudioChannelLabel>::size_type i = 0; i != channelLabels.size(); ++i)
		mChannelLayout->mChannelDescriptions[i].mChannelLabel = channelLabels[i];

	// Attempt to convert the channel labels to a layout tag
	AudioChannelLayoutTag tag;
	UInt32 dataSize = sizeof(tag);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_TagForChannelLayout, static_cast<UInt32>(AudioChannelLayoutSize(mChannelLayout)), mChannelLayout, &dataSize, &tag);
	if(result == noErr) {
		Reset(CreateChannelLayout(0));
		mChannelLayout->mChannelLayoutTag = tag;
	}
}

SFB::CAChannelLayout::CAChannelLayout(const AudioChannelLayout *rhs)
: mChannelLayout{CopyChannelLayout(rhs)}
{}

SFB::CAChannelLayout& SFB::CAChannelLayout::operator=(const AudioChannelLayout *rhs)
{
	Reset(CopyChannelLayout(rhs));
	return *this;
}

bool SFB::CAChannelLayout::operator==(const CAChannelLayout& rhs) const noexcept
{
	// Two empty channel layouts are considered equivalent
	if(!mChannelLayout && !rhs.mChannelLayout)
		return true;

	if(!mChannelLayout || !rhs.mChannelLayout)
		return false;

	const AudioChannelLayout *layouts [] = {
		rhs.mChannelLayout,
		mChannelLayout
	};

	UInt32 layoutsEqual = false;
	UInt32 propertySize = sizeof(layoutsEqual);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_AreChannelLayoutsEquivalent, sizeof(layouts), static_cast<void *>(layouts), &propertySize, &layoutsEqual);

	if(noErr != result)
		return false;
	//os_log_error(OS_LOG_DEFAULT, "AudioFormatGetProperty (kAudioFormatProperty_AreChannelLayoutsEquivalent) failed: %d", result);

	return layoutsEqual;
}

UInt32 SFB::CAChannelLayout::ChannelCount() const noexcept
{
	if(!mChannelLayout)
		return 0;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelDescriptions)
		return mChannelLayout->mNumberChannelDescriptions;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelBitmap)
		return static_cast<UInt32>(__builtin_popcount(mChannelLayout->mChannelBitmap));

	return AudioChannelLayoutTag_GetNumberOfChannels(mChannelLayout->mChannelLayoutTag);
}

bool SFB::CAChannelLayout::MapToLayout(const CAChannelLayout& outputLayout, std::vector<SInt32>& channelMap) const
{
	// No valid map exists for empty/unknown layouts
	if(!mChannelLayout || !outputLayout.mChannelLayout)
		return false;

	const AudioChannelLayout *layouts [] = {
		mChannelLayout,
		outputLayout.mChannelLayout
	};

	auto outputChannelCount = outputLayout.ChannelCount();
	if(outputChannelCount == 0)
		return false;

	SInt32 rawChannelMap [outputChannelCount];
	UInt32 propertySize = static_cast<UInt32>(sizeof(rawChannelMap));
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_ChannelMap, sizeof(layouts), static_cast<void *>(layouts), &propertySize, &rawChannelMap);

	if(noErr != result)
		return false;
	//os_log_error(OS_LOG_DEFAULT, "AudioFormatGetProperty (kAudioFormatProperty_ChannelMap) failed: %d", result);

	auto start = static_cast<SInt32 *>(rawChannelMap);
	channelMap.assign(start, start + outputChannelCount);

	return true;
}
