//
// Copyright © 2014-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <libkern/OSByteOrder.h>

#import "SFBCAStreamBasicDescription.hpp"

namespace {

/// Returns a descriptive format name for @c formatID or @c nullptr if unknown
CFStringRef _Nullable GetFormatIDName(AudioFormatID formatID) noexcept
{
	switch(formatID) {
		case kAudioFormatLinearPCM: 			return CFSTR("Linear PCM");
		case kAudioFormatAC3: 					return CFSTR("AC-3");
		case kAudioFormat60958AC3: 				return CFSTR("AC-3 over IEC 60958");
		case kAudioFormatAppleIMA4: 			return CFSTR("IMA 4:1 ADPCM");
		case kAudioFormatMPEG4AAC: 				return CFSTR("MPEG-4 Low Complexity AAC");
		case kAudioFormatMPEG4CELP: 			return CFSTR("MPEG-4 CELP");
		case kAudioFormatMPEG4HVXC: 			return CFSTR("MPEG-4 HVXC");
		case kAudioFormatMPEG4TwinVQ: 			return CFSTR("MPEG-4 TwinVQ");
		case kAudioFormatMACE3: 				return CFSTR("MACE 3:1");
		case kAudioFormatMACE6: 				return CFSTR("MACE 6:1");
		case kAudioFormatULaw: 					return CFSTR("µ-law 2:1");
		case kAudioFormatALaw: 					return CFSTR("A-law 2:1");
		case kAudioFormatQDesign: 				return CFSTR("QDesign music");
		case kAudioFormatQDesign2: 				return CFSTR("QDesign2 music");
		case kAudioFormatQUALCOMM :				return CFSTR("QUALCOMM PureVoice");
		case kAudioFormatMPEGLayer1: 			return CFSTR("MPEG-1/2 Layer I");
		case kAudioFormatMPEGLayer2: 			return CFSTR("MPEG-1/2 Layer II");
		case kAudioFormatMPEGLayer3: 			return CFSTR("MPEG-1/2 Layer III");
		case kAudioFormatTimeCode: 				return CFSTR("Stream of IOAudioTimeStamps");
		case kAudioFormatMIDIStream: 			return CFSTR("Stream of MIDIPacketLists");
		case kAudioFormatParameterValueStream:	return CFSTR("Float32 side-chain");
		case kAudioFormatAppleLossless: 		return CFSTR("Apple Lossless");
		case kAudioFormatMPEG4AAC_HE: 			return CFSTR("MPEG-4 High Efficiency AAC");
		case kAudioFormatMPEG4AAC_LD: 			return CFSTR("MPEG-4 AAC Low Delay");
		case kAudioFormatMPEG4AAC_ELD: 			return CFSTR("MPEG-4 AAC Enhanced Low Delay");
		case kAudioFormatMPEG4AAC_ELD_SBR: 		return CFSTR("MPEG-4 AAC Enhanced Low Delay with SBR extension");
		case kAudioFormatMPEG4AAC_ELD_V2: 		return CFSTR("MPEG-4 AAC Enhanced Low Delay Version 2");
		case kAudioFormatMPEG4AAC_HE_V2: 		return CFSTR("MPEG-4 High Efficiency AAC Version 2");
		case kAudioFormatMPEG4AAC_Spatial: 		return CFSTR("MPEG-4 Spatial Audio");
		case kAudioFormatMPEGD_USAC: 			return CFSTR("MPEG-D Unified Speech and Audio Coding");
		case kAudioFormatAMR: 					return CFSTR("AMR Narrow Band");
		case kAudioFormatAMR_WB: 				return CFSTR("AMR Wide Band");
		case kAudioFormatAudible: 				return CFSTR("Audible");
		case kAudioFormatiLBC: 					return CFSTR("iLBC narrow band");
		case kAudioFormatDVIIntelIMA: 			return CFSTR("DVI/Intel IMA ADPCM");
		case kAudioFormatMicrosoftGSM: 			return CFSTR("Microsoft GSM 6.10");
		case kAudioFormatAES3: 					return CFSTR("AES3-2003");
		case kAudioFormatEnhancedAC3: 			return CFSTR("Enhanced AC-3");
		case kAudioFormatFLAC: 					return CFSTR("Free Lossless Audio Codec");
		case kAudioFormatOpus: 					return CFSTR("Opus");
		case kAudioFormatAPAC: 					return CFSTR("Apple Positional Audio Codec");
		default:								return nullptr;
	}
}

/// Returns `true` if `c` is a printable ASCII character
constexpr bool IsPrintableASCII(unsigned char c) noexcept
{
	return c > 0x1f && c < 0x7f;
}

/// Creates a string representation of the four-character code @c fourcc
CFStringRef _Nullable CreateFourCharCodeString(UInt32 fourcc) noexcept CF_RETURNS_RETAINED
{
	union { UInt32 ui32; unsigned char str[4]; } u;
	u.ui32 = OSSwapHostToBigInt32(fourcc);

	if(IsPrintableASCII(u.str[0]) && IsPrintableASCII(u.str[1]) && IsPrintableASCII(u.str[2]) && IsPrintableASCII(u.str[3]))
		return CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("'%.4s'"), u.str);
	else
		return CFStringCreateWithFormat(kCFAllocatorDefault, nullptr, CFSTR("0x%.02x%.02x%.02x%.02x"), u.str[0], u.str[1], u.str[2], u.str[3]);
}

} // namespace

std::optional<SFB::CommonPCMFormat> SFB::CAStreamBasicDescription::CommonFormat() const noexcept
{
	if(mFramesPerPacket != 1 || mBytesPerFrame != mBytesPerPacket || mChannelsPerFrame == 0)
		return std::nullopt;

	if(!IsPCM() || !IsNativeEndian() || !IsImplicitlyPacked())
		return std::nullopt;

	if(IsSignedInteger()) {
		if(IsFixedPoint())
			return std::nullopt;

		if(mBitsPerChannel == 16)
			return CommonPCMFormat::int16;
		else if(mBitsPerChannel == 32)
			return CommonPCMFormat::int32;
	}
	else if(IsFloat()) {
		if(mBitsPerChannel == 32)
			return CommonPCMFormat::float32;
		else if(mBitsPerChannel == 64)
			return CommonPCMFormat::float64;
	}

	return std::nullopt;
}

// Most of this is stolen from Apple's CAStreamBasicDescription::Print()
CFStringRef SFB::CAStreamBasicDescription::CreateDescription() const noexcept
{
	CFMutableStringRef result = CFStringCreateMutable(kCFAllocatorDefault, 0);

	// Channels and sample rate
	if(std::rint(mSampleRate) == mSampleRate)
		CFStringAppendFormat(result, nullptr, CFSTR("%u ch @ %lld Hz, "), mChannelsPerFrame, static_cast<uint64_t>(mSampleRate));
	else
		CFStringAppendFormat(result, nullptr, CFSTR("%u ch @ %.2f Hz, "), mChannelsPerFrame, mSampleRate);

	// Shorter description for common formats
	if(auto commonFormat = CommonFormat(); commonFormat.has_value()) {
		switch(commonFormat.value()) {
			case CommonPCMFormat::int16:
				CFStringAppendCString(result, "Int16, ", kCFStringEncodingASCII);
				break;
			case CommonPCMFormat::int32:
				CFStringAppendCString(result, "Int32, ", kCFStringEncodingASCII);
				break;
			case CommonPCMFormat::float32:
				CFStringAppendCString(result, "Float32, ", kCFStringEncodingASCII);
				break;
			case CommonPCMFormat::float64:
				CFStringAppendCString(result, "Float64, ", kCFStringEncodingASCII);
				break;
		}

		if(IsNonInterleaved())
			CFStringAppendCString(result, "deinterleaved", kCFStringEncodingASCII);
		else
			CFStringAppendCString(result, "interleaved", kCFStringEncodingASCII);

		return result;
	}

	if(IsPCM()) {
		// Bit depth
		auto fractionalBits = FractionalBits();
		if(fractionalBits > 0)
			CFStringAppendFormat(result, nullptr, CFSTR("%d.%d-bit"), mBitsPerChannel - fractionalBits, fractionalBits);
		else
			CFStringAppendFormat(result, nullptr, CFSTR("%d-bit"), mBitsPerChannel);

		// Endianness
		auto sampleWordSize = SampleWordSize();
		if(sampleWordSize > 1)
			CFStringAppendCString(result, IsBigEndian() ? " big-endian" : " little-endian", kCFStringEncodingASCII);

		// Sign
		auto isInteger = IsInteger();
		if(isInteger)
			CFStringAppendCString(result, IsSignedInteger() ? " signed" : " unsigned", kCFStringEncodingASCII);

		// Integer or floating
		CFStringAppendCString(result, isInteger ? " integer" : " float", kCFStringEncodingASCII);

		// Packedness and alignment
		if(sampleWordSize > 0) {
			if(IsImplicitlyPacked())
				CFStringAppendCString(result, ", packed", kCFStringEncodingASCII);
			else if(IsUnaligned())
				CFStringAppendCString(result, IsAlignedHigh() ? ", high-aligned" : ", low-aligned", kCFStringEncodingASCII);

			CFStringAppendFormat(result, nullptr, CFSTR(" in %d bytes"), sampleWordSize);
		}

		if(IsNonInterleaved())
			CFStringAppendCString(result, ", deinterleaved", kCFStringEncodingASCII);
	}
	else if(mFormatID == kAudioFormatAppleLossless || mFormatID == kAudioFormatFLAC) {
		if(CFStringRef formatIDString = GetFormatIDName(mFormatID); formatIDString)
			CFStringAppend(result, formatIDString);
		else if(CFStringRef fourCC = CreateFourCharCodeString(mFormatID); fourCC) {
			CFStringAppend(result, fourCC);
			CFRelease(fourCC);
		}
		else
			CFStringAppendFormat(result, nullptr, CFSTR("0x%.08x"), mFormatID);

		CFStringAppendCString(result, ", ", kCFStringEncodingASCII);

		UInt32 sourceBitDepth = 0;
		switch(mFormatFlags) {
			case kAppleLosslessFormatFlag_16BitSourceData:	sourceBitDepth = 16;	break;
			case kAppleLosslessFormatFlag_20BitSourceData:	sourceBitDepth = 20;	break;
			case kAppleLosslessFormatFlag_24BitSourceData:	sourceBitDepth = 24;	break;
			case kAppleLosslessFormatFlag_32BitSourceData:	sourceBitDepth = 32;	break;
		}

		if(sourceBitDepth != 0)
			CFStringAppendFormat(result, nullptr, CFSTR("from %d-bit source, "), sourceBitDepth);
		else
			CFStringAppendCString(result, "from UNKNOWN source bit depth, ", kCFStringEncodingASCII);

		CFStringAppendFormat(result, nullptr, CFSTR("%d frames/packet"), mFramesPerPacket);
	}
	else {
		if(CFStringRef formatIDString = GetFormatIDName(mFormatID); formatIDString)
			CFStringAppend(result, formatIDString);
		else if(CFStringRef fourCC = CreateFourCharCodeString(mFormatID); fourCC) {
			CFStringAppend(result, fourCC);
			CFRelease(fourCC);
		}
		else
			CFStringAppendFormat(result, nullptr, CFSTR("0x%.08x"), mFormatID);

		// Format flags
		if(mFormatFlags != 0)
			CFStringAppendFormat(result, nullptr, CFSTR(" [0x%.08x]"), mFormatFlags);

		CFStringAppendFormat(result, nullptr, CFSTR(", %u bits/channel, %u bytes/packet, %u frames/packet, %u bytes/frame"), mBitsPerChannel, mBytesPerPacket, mFramesPerPacket, mBytesPerFrame);
	}

	return result;
}
