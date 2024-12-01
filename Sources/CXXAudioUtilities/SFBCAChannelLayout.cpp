//
// Copyright (c) 2013 - 2024 Stephen F. Booth <me@sbooth.org>
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
		case kAudioChannelLayoutTag_Mono:
			return "Mono";
		case kAudioChannelLayoutTag_Stereo:
			return "Stereo";
		case kAudioChannelLayoutTag_StereoHeadphones:
			return "Stereo Headphones";
		case kAudioChannelLayoutTag_MatrixStereo:
			return "Matrix Stereo";
		case kAudioChannelLayoutTag_MidSide:
			return "Mid-Side";
		case kAudioChannelLayoutTag_XY:
			return "XY";
		case kAudioChannelLayoutTag_Binaural:
			return "Binaural";
		case kAudioChannelLayoutTag_Ambisonic_B_Format:
			return "Ambisonic B-format";
		case kAudioChannelLayoutTag_Quadraphonic:
			return "Quadraphonic";
		case kAudioChannelLayoutTag_Pentagonal:
			return "Pentagonal";
		case kAudioChannelLayoutTag_Hexagonal:
			return "Hexagonal";
		case kAudioChannelLayoutTag_Octagonal:
			return "Octagonal";
		case kAudioChannelLayoutTag_Cube:
			return "Cube";
		case kAudioChannelLayoutTag_MPEG_3_0_A:
			return "MPEG 3.0 A";
		case kAudioChannelLayoutTag_MPEG_3_0_B:
			return "MPEG 3.0 B";
		case kAudioChannelLayoutTag_MPEG_4_0_A:
			return "MPEG 4.0 A";
		case kAudioChannelLayoutTag_MPEG_4_0_B:
			return "MPEG 4.0 B";
		case kAudioChannelLayoutTag_MPEG_5_0_A:
			return "MPEG 5.0 A";
		case kAudioChannelLayoutTag_MPEG_5_0_B:
			return "MPEG 5.0 B";
		case kAudioChannelLayoutTag_MPEG_5_0_C:
			return "MPEG 5.0 C";
		case kAudioChannelLayoutTag_MPEG_5_0_D:
			return "MPEG 5.0 D";
		case kAudioChannelLayoutTag_MPEG_5_1_A:
			return "MPEG 5.1 A";
		case kAudioChannelLayoutTag_MPEG_5_1_B:
			return "MPEG 5.1 B";
		case kAudioChannelLayoutTag_MPEG_5_1_C:
			return "MPEG 5.1 C";
		case kAudioChannelLayoutTag_MPEG_5_1_D:
			return "MPEG 5.1 D";
		case kAudioChannelLayoutTag_MPEG_6_1_A:
			return "MPEG 6.1 A";
		case kAudioChannelLayoutTag_MPEG_7_1_A:
			return "MPEG 7.1 A";
		case kAudioChannelLayoutTag_MPEG_7_1_B:
			return "MPEG 7.1 B";
		case kAudioChannelLayoutTag_MPEG_7_1_C:
			return "MPEG 7.1 C";
		case kAudioChannelLayoutTag_Emagic_Default_7_1:
			return "Emagic Default 7.1";
		case kAudioChannelLayoutTag_SMPTE_DTV:
			return "SMPTE DTV";
		case kAudioChannelLayoutTag_ITU_2_1:
			return "ITU 2.1";
		case kAudioChannelLayoutTag_ITU_2_2:
			return "ITU 2.2";
		case kAudioChannelLayoutTag_DVD_4:
			return "DVD 4";
		case kAudioChannelLayoutTag_DVD_5:
			return "DVD 5";
		case kAudioChannelLayoutTag_DVD_6:
			return "DVD 6";
		case kAudioChannelLayoutTag_DVD_10:
			return "DVD 10";
		case kAudioChannelLayoutTag_DVD_11:
			return "DVD 11";
		case kAudioChannelLayoutTag_DVD_18:
			return "DVD 18";
		case kAudioChannelLayoutTag_AudioUnit_6_0:
			return "AudioUnit 6.0";
		case kAudioChannelLayoutTag_AudioUnit_7_0:
			return "AudioUnit 7.0";
		case kAudioChannelLayoutTag_AudioUnit_7_0_Front:
			return "AudioUnit 7.0 Front";
		case kAudioChannelLayoutTag_AAC_6_0:
			return "AAC 6.0";
		case kAudioChannelLayoutTag_AAC_6_1:
			return "AAC 6.1";
		case kAudioChannelLayoutTag_AAC_7_0:
			return "AAC 7.0";
		case kAudioChannelLayoutTag_AAC_7_1_B:
			return "AAC 7.1 B";
		case kAudioChannelLayoutTag_AAC_7_1_C:
			return "AAC 7.1 C";
		case kAudioChannelLayoutTag_AAC_Octagonal:
			return "AAC Octagonal";
		case kAudioChannelLayoutTag_TMH_10_2_std:
			return "TMH 10.2 standard";
		case kAudioChannelLayoutTag_TMH_10_2_full:
			return "TMH 10.2 full";
		case kAudioChannelLayoutTag_AC3_1_0_1:
			return "AC3 1.0.1";
		case kAudioChannelLayoutTag_AC3_3_0:
			return "AC3 3.0";
		case kAudioChannelLayoutTag_AC3_3_1:
			return "AC3 3.1";
		case kAudioChannelLayoutTag_AC3_3_0_1:
			return "AC3 3.0.1";
		case kAudioChannelLayoutTag_AC3_2_1_1:
			return "AC3 2.1.1";
		case kAudioChannelLayoutTag_AC3_3_1_1:
			return "AC3 3.1.1";
		case kAudioChannelLayoutTag_EAC_6_0_A:
			return "EAC 6.0 A";
		case kAudioChannelLayoutTag_EAC_7_0_A:
			return "EAC 7.0 A";
		case kAudioChannelLayoutTag_EAC3_6_1_A:
			return "EAC3 6.1 A";
		case kAudioChannelLayoutTag_EAC3_6_1_B:
			return "EAC3 6.1 B";
		case kAudioChannelLayoutTag_EAC3_6_1_C:
			return "EAC3 6.1 C";
		case kAudioChannelLayoutTag_EAC3_7_1_A:
			return "EAC3 7.1 A";
		case kAudioChannelLayoutTag_EAC3_7_1_B:
			return "EAC3 7.1 B";
		case kAudioChannelLayoutTag_EAC3_7_1_C:
			return "EAC3 7.1 C";
		case kAudioChannelLayoutTag_EAC3_7_1_D:
			return "EAC3 7.1 D";
		case kAudioChannelLayoutTag_EAC3_7_1_E:
			return "EAC3 7.1 E";
		case kAudioChannelLayoutTag_EAC3_7_1_F:
			return "EAC3 7.1 F";
		case kAudioChannelLayoutTag_EAC3_7_1_G:
			return "EAC3 7.1 G";
		case kAudioChannelLayoutTag_EAC3_7_1_H:
			return "EAC3 7.1 H";
		case kAudioChannelLayoutTag_DTS_3_1:
			return "DTS 3.1";
		case kAudioChannelLayoutTag_DTS_4_1:
			return "DTS 4.1";
		case kAudioChannelLayoutTag_DTS_6_0_A:
			return "DTS 6.0 A";
		case kAudioChannelLayoutTag_DTS_6_0_B:
			return "DTS 6.0 B";
		case kAudioChannelLayoutTag_DTS_6_0_C:
			return "DTS 6.0 C";
		case kAudioChannelLayoutTag_DTS_6_1_A:
			return "DTS 6.1 A";
		case kAudioChannelLayoutTag_DTS_6_1_B:
			return "DTS 6.1 B";
		case kAudioChannelLayoutTag_DTS_6_1_C:
			return "DTS 6.1 C";
		case kAudioChannelLayoutTag_DTS_7_0:
			return "DTS 7.0";
		case kAudioChannelLayoutTag_DTS_7_1:
			return "DTS 7.1";
		case kAudioChannelLayoutTag_DTS_8_0_A:
			return "DTS 8.0 A";
		case kAudioChannelLayoutTag_DTS_8_0_B:
			return "DTS 8.0 B";
		case kAudioChannelLayoutTag_DTS_8_1_A:
			return "DTS 8.1 A";
		case kAudioChannelLayoutTag_DTS_8_1_B:
			return "DTS 8.1 B";
		case kAudioChannelLayoutTag_DTS_6_1_D:
			return "DTS 6.1 D";
		case kAudioChannelLayoutTag_WAVE_4_0_B:
			return "WAVE 4.0 B";
		case kAudioChannelLayoutTag_WAVE_5_0_B:
			return "WAVE 5.0 B";
		case kAudioChannelLayoutTag_WAVE_5_1_B:
			return "WAVE 5.1 B";
		case kAudioChannelLayoutTag_WAVE_6_1:
			return "WAVE 6.1";
		case kAudioChannelLayoutTag_WAVE_7_1:
			return "WAVE 7.1";
		case kAudioChannelLayoutTag_Atmos_5_1_2:
			return "Atmos 5.1.2";
		case kAudioChannelLayoutTag_Atmos_5_1_4:
			return "Atmos 5.1.4";
		case kAudioChannelLayoutTag_Atmos_7_1_2:
			return "Atmos 7.1.2";
		case kAudioChannelLayoutTag_Atmos_7_1_4:
			return "Atmos 7.1.4";
		case kAudioChannelLayoutTag_Atmos_9_1_6:
			return "Atmos 9.1.6";
		case kAudioChannelLayoutTag_Logic_4_0_C:
			return "Logic 4.0 C";
		case kAudioChannelLayoutTag_Logic_6_0_B:
			return "Logic 6.0 B";
		case kAudioChannelLayoutTag_Logic_6_1_B:
			return "Logic 6.1 B";
		case kAudioChannelLayoutTag_Logic_6_1_D:
			return "Logic 6.1 D";
		case kAudioChannelLayoutTag_Logic_7_1_B:
			return "Logic 7.1 B";
		case kAudioChannelLayoutTag_Logic_Atmos_7_1_4_B:
			return "Logic Atmos 7.1.4 B";
		case kAudioChannelLayoutTag_Logic_Atmos_7_1_6:
			return "Logic Atmos 7.1.6";
		case kAudioChannelLayoutTag_CICP_13:
			return "CICP 13";
		case kAudioChannelLayoutTag_CICP_14:
			return "CICP 14";
		case kAudioChannelLayoutTag_CICP_15:
			return "CICP 15";
		case kAudioChannelLayoutTag_CICP_16:
			return "CICP 16";
		case kAudioChannelLayoutTag_CICP_17:
			return "CICP 17";
		case kAudioChannelLayoutTag_CICP_18:
			return "CICP 18";
		case kAudioChannelLayoutTag_CICP_19:
			return "CICP 19";
		case kAudioChannelLayoutTag_CICP_20:
			return "CICP 20";
		case kAudioChannelLayoutTag_Ogg_5_0:
			return "Ogg 5.0";
		case kAudioChannelLayoutTag_Ogg_5_1:
			return "Ogg 5.1";
		case kAudioChannelLayoutTag_Ogg_6_1:
			return "Ogg 6.1";
		case kAudioChannelLayoutTag_Ogg_7_1:
			return "Ogg 7.1";
		case kAudioChannelLayoutTag_MPEG_5_0_E:
			return "MPEG 5.0 E";
		case kAudioChannelLayoutTag_MPEG_5_1_E:
			return "MPEG 5.1 E";
		case kAudioChannelLayoutTag_MPEG_6_1_B:
			return "MPEG 6.1 B";
		case kAudioChannelLayoutTag_MPEG_7_1_D:
			return "MPEG 7.1 D";

		case kAudioChannelLayoutTag_BeginReserved...kAudioChannelLayoutTag_EndReserved:
			return "Reserved";

		default:
			break;
	}

	switch(layoutTag & 0xFFFF0000) {
		case kAudioChannelLayoutTag_HOA_ACN_SN3D:
			return "HOA ACN SN3D";
		case kAudioChannelLayoutTag_HOA_ACN_N3D:
			return "HOA ACN N3D";
		case kAudioChannelLayoutTag_DiscreteInOrder:
			return "Discrete in Order";
		case kAudioChannelLayoutTag_Unknown:
			return "Unknown";

		default:
			break;
	}

	return nullptr;
}

/// Returns the string representation of an @c AudioChannelLabel
constexpr const char * GetChannelLabelName(AudioChannelLabel label) noexcept
{
	switch(label) {
		case kAudioChannelLabel_Unknown:
			return "Unknown";
		case kAudioChannelLabel_Unused:
			return "Unused";
		case kAudioChannelLabel_UseCoordinates:
			return "Use Coordinates";
		case kAudioChannelLabel_Left:
			return "Left";
		case kAudioChannelLabel_Right:
			return "Right";
		case kAudioChannelLabel_Center:
			return "Center";
		case kAudioChannelLabel_LFEScreen:
			return "LFE Screen";
		case kAudioChannelLabel_LeftSurround:
			return "Left Surround";
		case kAudioChannelLabel_RightSurround:
			return "Right Surround";
		case kAudioChannelLabel_LeftCenter:
			return "Left Center";
		case kAudioChannelLabel_RightCenter:
			return "Right Center";
		case kAudioChannelLabel_CenterSurround:
			return "Center Surround";
		case kAudioChannelLabel_LeftSurroundDirect:
			return "Left Surround Direct";
		case kAudioChannelLabel_RightSurroundDirect:
			return "Right Surround Direct";
		case kAudioChannelLabel_TopCenterSurround:
			return "Top Center Surround";
		case kAudioChannelLabel_VerticalHeightLeft:
			return "Vertical Height Left";
		case kAudioChannelLabel_VerticalHeightCenter:
			return "Vertical Height Center";
		case kAudioChannelLabel_VerticalHeightRight:
			return "Vertical Height Right";
		case kAudioChannelLabel_TopBackLeft:
			return "Top Back Left";
		case kAudioChannelLabel_TopBackCenter:
			return "Top Back Center";
		case kAudioChannelLabel_TopBackRight:
			return "Top Back Right";
		case kAudioChannelLabel_RearSurroundLeft:
			return "Rear Surround Left";
		case kAudioChannelLabel_RearSurroundRight:
			return "Rear Surround Right";
		case kAudioChannelLabel_LeftWide:
			return "Left Wide";
		case kAudioChannelLabel_RightWide:
			return "Right Wide";
		case kAudioChannelLabel_LFE2:
			return "LFE2";
		case kAudioChannelLabel_LeftTotal:
			return "Left Total";
		case kAudioChannelLabel_RightTotal:
			return "Right Total";
		case kAudioChannelLabel_HearingImpaired:
			return "Hearing Impaired";
		case kAudioChannelLabel_Narration:
			return "Narration";
		case kAudioChannelLabel_Mono:
			return "Mono";
		case kAudioChannelLabel_DialogCentricMix:
			return "Dialog Centric Mix";
		case kAudioChannelLabel_CenterSurroundDirect:
			return "Center Surround Direct";
		case kAudioChannelLabel_Haptic:
			return "Haptic";
		case kAudioChannelLabel_LeftTopMiddle:
			return "Left Top Middle";
		case kAudioChannelLabel_RightTopMiddle:
			return "Right Top Middle";
		case kAudioChannelLabel_LeftTopRear:
			return "Left Top Rear";
		case kAudioChannelLabel_CenterTopRear:
			return "Center Top Rear";
		case kAudioChannelLabel_RightTopRear:
			return "Right Top Rear";
		case kAudioChannelLabel_LeftSideSurround:
			return "Left Side Surround";
		case kAudioChannelLabel_RightSideSurround:
			return "Right Side Surround";
		case kAudioChannelLabel_LeftBottom:
			return "Left Bottom";
		case kAudioChannelLabel_RightBottom:
			return "Right Bottom";
		case kAudioChannelLabel_CenterBottom:
			return "Center Bottom";
		case kAudioChannelLabel_LeftTopSurround:
			return "Left Top Surround";
		case kAudioChannelLabel_RightTopSurround:
			return "Right Top Surround";
		case kAudioChannelLabel_LFE3:
			return "LFE3";
		case kAudioChannelLabel_LeftBackSurround:
			return "Left Back Surround";
		case kAudioChannelLabel_RightBackSurround:
			return "Right Back Surround";
		case kAudioChannelLabel_LeftEdgeOfScreen:
			return "Left Edge of Screen";
		case kAudioChannelLabel_RightEdgeOfScreen:
			return "Right Edge of Screen";
		case kAudioChannelLabel_Ambisonic_W:
			return "Ambisonic W";
		case kAudioChannelLabel_Ambisonic_X:
			return "Ambisonic X";
		case kAudioChannelLabel_Ambisonic_Y:
			return "Ambisonic Y";
		case kAudioChannelLabel_Ambisonic_Z:
			return "Ambisonic Z";
		case kAudioChannelLabel_MS_Mid:
			return "MS Mid";
		case kAudioChannelLabel_MS_Side:
			return "MS Side";
		case kAudioChannelLabel_XY_X:
			return "XY X";
		case kAudioChannelLabel_XY_Y:
			return "XY Y";
		case kAudioChannelLabel_BinauralLeft:
			return "Binaural Left";
		case kAudioChannelLabel_BinauralRight:
			return "Binaural Right";
		case kAudioChannelLabel_HeadphonesLeft:
			return "Headphones Left";
		case kAudioChannelLabel_HeadphonesRight:
			return "Headphones Right";
		case kAudioChannelLabel_ClickTrack:
			return "Click Track";
		case kAudioChannelLabel_ForeignLanguage:
			return "Foreign Language";
		case kAudioChannelLabel_Discrete:
			return "Discrete";
		case kAudioChannelLabel_Discrete_0:
			return "Discrete 0";
		case kAudioChannelLabel_Discrete_1:
			return "Discrete 1";
		case kAudioChannelLabel_Discrete_2:
			return "Discrete 2";
		case kAudioChannelLabel_Discrete_3:
			return "Discrete 3";
		case kAudioChannelLabel_Discrete_4:
			return "Discrete 4";
		case kAudioChannelLabel_Discrete_5:
			return "Discrete 5";
		case kAudioChannelLabel_Discrete_6:
			return "Discrete 6";
		case kAudioChannelLabel_Discrete_7:
			return "Discrete 7";
		case kAudioChannelLabel_Discrete_8:
			return "Discrete 8";
		case kAudioChannelLabel_Discrete_9:
			return "Discrete 9";
		case kAudioChannelLabel_Discrete_10:
			return "Discrete 10";
		case kAudioChannelLabel_Discrete_11:
			return "Discrete 11";
		case kAudioChannelLabel_Discrete_12:
			return "Discrete 12";
		case kAudioChannelLabel_Discrete_13:
			return "Discrete 13";
		case kAudioChannelLabel_Discrete_14:
			return "Discrete 14";
		case kAudioChannelLabel_Discrete_15:
			return "Discrete 15";
		case kAudioChannelLabel_Discrete_65535:
			return "Discrete 65535";
		case kAudioChannelLabel_HOA_ACN:
			return "HOA ACN";
		case kAudioChannelLabel_HOA_ACN_0:
			return "HOA ACN 0";
		case kAudioChannelLabel_HOA_ACN_1:
			return "HOA ACN 1";
		case kAudioChannelLabel_HOA_ACN_2:
			return "HOA ACN 2";
		case kAudioChannelLabel_HOA_ACN_3:
			return "HOA ACN 3";
		case kAudioChannelLabel_HOA_ACN_4:
			return "HOA ACN 4";
		case kAudioChannelLabel_HOA_ACN_5:
			return "HOA ACN 5";
		case kAudioChannelLabel_HOA_ACN_6:
			return "HOA ACN 6";
		case kAudioChannelLabel_HOA_ACN_7:
			return "HOA ACN 7";
		case kAudioChannelLabel_HOA_ACN_8:
			return "HOA ACN 8";
		case kAudioChannelLabel_HOA_ACN_9:
			return "HOA ACN 9";
		case kAudioChannelLabel_HOA_ACN_10:
			return "HOA ACN 10";
		case kAudioChannelLabel_HOA_ACN_11:
			return "HOA ACN 11";
		case kAudioChannelLabel_HOA_ACN_12:
			return "HOA ACN 12";
		case kAudioChannelLabel_HOA_ACN_13:
			return "HOA ACN 13";
		case kAudioChannelLabel_HOA_ACN_14:
			return "HOA ACN 14";
		case kAudioChannelLabel_HOA_ACN_15:
			return "HOA ACN 15";
		case kAudioChannelLabel_HOA_ACN_65024:
			return "HOA ACN 65024";

		case kAudioChannelLabel_BeginReserved...kAudioChannelLabel_EndReserved:
			return "Reserved";

		default:
			break;
	}

	switch(label & 0xFFFF0000) {
		case kAudioChannelLabel_HOA_N3D:
			return "HOA N3D";
		case kAudioChannelLabel_Object:
			return "Object";

		default:
			break;
	}

	return nullptr;
}

}

size_t SFB::AudioChannelLayoutSize(const AudioChannelLayout *channelLayout) noexcept
{
	if(!channelLayout)
		return 0;
	return ChannelLayoutSize(channelLayout->mNumberChannelDescriptions);
}

// Constants
const SFB::CAChannelLayout SFB::CAChannelLayout::Mono		= CAChannelLayout(kAudioChannelLayoutTag_Mono);
const SFB::CAChannelLayout SFB::CAChannelLayout::Stereo 	= CAChannelLayout(kAudioChannelLayoutTag_Stereo);

SFB::CAChannelLayout SFB::CAChannelLayout::ChannelLayoutWithBitmap(UInt32 channelBitmap)
{
	CAChannelLayout channelLayout{};
	channelLayout.mChannelLayout = CreateChannelLayout(0);
	channelLayout.mChannelLayout->mChannelLayoutTag = kAudioChannelLayoutTag_UseChannelBitmap;
	channelLayout.mChannelLayout->mChannelBitmap = channelBitmap;
	return channelLayout;
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

size_t SFB::CAChannelLayout::ChannelCount() const noexcept
{
	if(!mChannelLayout)
		return 0;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelDescriptions)
		return mChannelLayout->mNumberChannelDescriptions;

	if(mChannelLayout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelBitmap)
		return (size_t)__builtin_popcount(mChannelLayout->mChannelBitmap);

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
	UInt32 propertySize = (UInt32)sizeof(rawChannelMap);
	OSStatus result = AudioFormatGetProperty(kAudioFormatProperty_ChannelMap, sizeof(layouts), static_cast<void *>(layouts), &propertySize, &rawChannelMap);

	if(noErr != result)
		return false;
	//os_log_error(OS_LOG_DEFAULT, "AudioFormatGetProperty (kAudioFormatProperty_ChannelMap) failed: %d", result);

	auto start = static_cast<SInt32 *>(rawChannelMap);
	channelMap.assign(start, start + outputChannelCount);

	return true;
}

SFB::CFString SFB::CAChannelLayout::Description(const char * const prefix) const noexcept
{
	if(!mChannelLayout) {
		if(prefix)
			return CFString(prefix, kCFStringEncodingUTF8);
		else
			return CFString{};
	}

	CFMutableString result{ CFStringCreateMutable(kCFAllocatorDefault, 0) };

	if(prefix)
		CFStringAppendCString(result, prefix, kCFStringEncodingUTF8);

	if(kAudioChannelLayoutTag_UseChannelBitmap == mChannelLayout->mChannelLayoutTag)
		CFStringAppendFormat(result, NULL, CFSTR("Channel bitmap: 0x%0.8x"), mChannelLayout->mChannelBitmap);
	else if(kAudioChannelLayoutTag_UseChannelDescriptions == mChannelLayout->mChannelLayoutTag){
		CFStringAppendFormat(result, NULL, CFSTR("%u channels ["), mChannelLayout->mNumberChannelDescriptions);

		const AudioChannelDescription *desc = mChannelLayout->mChannelDescriptions;
		for(UInt32 i = 0; i < mChannelLayout->mNumberChannelDescriptions; ++i, ++desc) {
			if(kAudioChannelLabel_UseCoordinates == desc->mChannelLabel)
				CFStringAppendFormat(result, NULL, CFSTR("(%f, %f, %f), flags = 0x%0.8x"), desc->mCoordinates[0], desc->mCoordinates[1], desc->mCoordinates[2], desc->mChannelFlags);
			else
				CFStringAppendFormat(result, NULL, CFSTR("%s (0x%0.8x)"), GetChannelLabelName(desc->mChannelLabel), desc->mChannelLabel);
			if(i < mChannelLayout->mNumberChannelDescriptions - 1)
				CFStringAppend(result, CFSTR(", "));
		}

		CFStringAppend(result, CFSTR("]"));
	}
	else
		CFStringAppendFormat(result, NULL, CFSTR("%s (0x%0.8x)"), GetChannelLayoutTagName(mChannelLayout->mChannelLayoutTag), mChannelLayout->mChannelLayoutTag);

	return CFString(static_cast<CFStringRef>(result.Relinquish()));
}
