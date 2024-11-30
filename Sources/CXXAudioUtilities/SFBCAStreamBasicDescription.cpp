//
// Copyright (c) 2014 - 2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <libkern/OSByteOrder.h>

#import "SFBCAStreamBasicDescription.hpp"

// Most of this is stolen from Apple's CAStreamBasicDescription::Print()
std::string SFB::CAStreamBasicDescription::Description(const char * const prefix) const noexcept
{
	std::string result{};

	if(prefix)
		result.append(prefix);

	char buf [128];

	unsigned char formatID [5];
	auto formatIDBE = OSSwapHostToBigInt32(mFormatID);
	std::memcpy(formatID, &formatIDBE, 4);
	formatID[4] = '\0';

	// General description
	snprintf(buf, sizeof(buf), "%u ch, %.2f Hz, '%.4s' (0x%0.8x) ", mChannelsPerFrame, mSampleRate, formatID, mFormatFlags);
	result.append(buf);

	if(kAudioFormatLinearPCM == mFormatID) {
		// Bit depth
		UInt32 fractionalBits = (kLinearPCMFormatFlagsSampleFractionMask & mFormatFlags) >> kLinearPCMFormatFlagsSampleFractionShift;
		if(fractionalBits > 0)
			snprintf(buf, sizeof(buf), "%d.%d-bit", mBitsPerChannel - fractionalBits, fractionalBits);
		else
			snprintf(buf, sizeof(buf), "%d-bit", mBitsPerChannel);
		result.append(buf);

		// Endianness
		bool isInterleaved = !(kAudioFormatFlagIsNonInterleaved & mFormatFlags);
		UInt32 interleavedChannelCount = (isInterleaved ? mChannelsPerFrame : 1);
		UInt32 sampleSize = (mBytesPerFrame > 0 && interleavedChannelCount > 0 ? mBytesPerFrame / interleavedChannelCount : 0);
		if(sampleSize > 1)
			result.append((kLinearPCMFormatFlagIsBigEndian & mFormatFlags) ? " big-endian" : " little-endian");

		// Sign
		bool isInteger = !(kLinearPCMFormatFlagIsFloat & mFormatFlags);
		if(isInteger)
			result.append((kLinearPCMFormatFlagIsSignedInteger & mFormatFlags) ? " signed" : " unsigned");

		// Integer or floating
		result.append(isInteger ? " integer" : " float");

		// Packedness
		if(sampleSize > 0 && ((sampleSize << 3) != mBitsPerChannel)) {
			snprintf(buf, sizeof(buf), ", %s in %d bytes", (kLinearPCMFormatFlagIsPacked & mFormatFlags) ? "packed" : "unpacked", sampleSize);
			result.append(buf);
		}

		// Alignment
		if((sampleSize > 0 && ((sampleSize << 3) != mBitsPerChannel)) || ((mBitsPerChannel & 7) != 0))
			result.append((kLinearPCMFormatFlagIsAlignedHigh & mFormatFlags) ? " high-aligned" : " low-aligned");

		if(!isInterleaved)
			result.append(", deinterleaved");
	}
	else if(kAudioFormatAppleLossless == mFormatID) {
		UInt32 sourceBitDepth = 0;
		switch(mFormatFlags) {
			case kAppleLosslessFormatFlag_16BitSourceData:		sourceBitDepth = 16;	break;
			case kAppleLosslessFormatFlag_20BitSourceData:		sourceBitDepth = 20;	break;
			case kAppleLosslessFormatFlag_24BitSourceData:		sourceBitDepth = 24;	break;
			case kAppleLosslessFormatFlag_32BitSourceData:		sourceBitDepth = 32;	break;
		}

		if(sourceBitDepth != 0)
			snprintf(buf, sizeof(buf), "from %d-bit source, ", sourceBitDepth);
		else
			snprintf(buf, sizeof(buf), "from UNKNOWN source bit depth, ");
		result.append(buf);

		snprintf(buf, sizeof(buf), " %d frames/packet", mFramesPerPacket);
		result.append(buf);
	}
	else {
		snprintf(buf, sizeof(buf), "%u bits/channel, %u bytes/packet, %u frames/packet, %u bytes/frame", mBitsPerChannel, mBytesPerPacket, mFramesPerPacket, mBytesPerFrame);
		result.append(buf);
	}

	return result;
}
