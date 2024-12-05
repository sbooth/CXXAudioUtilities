//
// Copyright © 2014-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#import <cstdio>

#import <libkern/OSByteOrder.h>

#import "SFBCAStreamBasicDescription.hpp"

#import "fmt/format.h"

// Most of this is stolen from Apple's CAStreamBasicDescription::Print()
std::string SFB::CAStreamBasicDescription::Description() const
{
	auto out = fmt::memory_buffer();

	char formatID [5];
	auto formatIDBE = OSSwapHostToBigInt32(mFormatID);
	std::memcpy(formatID, &formatIDBE, 4);
	formatID[4] = '\0';

	// General description
	fmt::format_to(std::back_inserter(out), "{} ch, {} Hz, '{:.4}' ({:#010x}) ", mChannelsPerFrame, mSampleRate, formatID, mFormatFlags);

	if(kAudioFormatLinearPCM == mFormatID) {
		// Bit depth
		UInt32 fractionalBits = (kLinearPCMFormatFlagsSampleFractionMask & mFormatFlags) >> kLinearPCMFormatFlagsSampleFractionShift;
		if(fractionalBits > 0)
			fmt::format_to(std::back_inserter(out), "{}.{}-bit", mBitsPerChannel - fractionalBits, fractionalBits);
		else
			fmt::format_to(std::back_inserter(out), "{}-bit", mBitsPerChannel);

		// Endianness
		bool isInterleaved = !(kAudioFormatFlagIsNonInterleaved & mFormatFlags);
		UInt32 interleavedChannelCount = (isInterleaved ? mChannelsPerFrame : 1);
		UInt32 sampleSize = (mBytesPerFrame > 0 && interleavedChannelCount > 0 ? mBytesPerFrame / interleavedChannelCount : 0);
		if(sampleSize > 1)
			fmt::format_to(std::back_inserter(out), (kLinearPCMFormatFlagIsBigEndian & mFormatFlags) ? " big-endian" : " little-endian");

		// Sign
		bool isInteger = !(kLinearPCMFormatFlagIsFloat & mFormatFlags);
		if(isInteger)
			fmt::format_to(std::back_inserter(out), (kLinearPCMFormatFlagIsSignedInteger & mFormatFlags) ? " signed" : " unsigned");

		// Integer or floating
		fmt::format_to(std::back_inserter(out), isInteger ? " integer" : " float");

		// Packedness
		if(sampleSize > 0 && ((sampleSize << 3) != mBitsPerChannel))
			fmt::format_to(std::back_inserter(out), ", {} in {} bytes", (kLinearPCMFormatFlagIsPacked & mFormatFlags) ? "packed" : "unpacked", sampleSize);

		// Alignment
		if((sampleSize > 0 && ((sampleSize << 3) != mBitsPerChannel)) || ((mBitsPerChannel & 7) != 0))
			fmt::format_to(std::back_inserter(out), (kLinearPCMFormatFlagIsAlignedHigh & mFormatFlags) ? " high-aligned" : " low-aligned");

		if(!isInterleaved)
			fmt::format_to(std::back_inserter(out), ", deinterleaved");
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
			fmt::format_to(std::back_inserter(out), "from {}-bit source, ", sourceBitDepth);
		else
			fmt::format_to(std::back_inserter(out), "from UNKNOWN source bit depth, ");

		fmt::format_to(std::back_inserter(out), " {} frames/packet", mFramesPerPacket);
	}
	else
		fmt::format_to(std::back_inserter(out),
					   "{} bits/channel, {} bytes/packet, {} frames/packet, {} bytes/frame",
					   mBitsPerChannel,
					   mBytesPerPacket,
					   mFramesPerPacket,
					   mBytesPerFrame);

	return fmt::to_string(out);
}
