//
// Copyright © 2021-2024 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import "SFBCAAudioObject.hpp"
#import "SFBCAStreamBasicDescription.hpp"

namespace SFB {

class CAAudioStream : public CAAudioObject
{

public:

	/// Creates an unknown @c CAAudioStream
	CAAudioStream() noexcept = default;

	/// Copy constructor
	CAAudioStream(const CAAudioStream& rhs) noexcept = default;

	/// Assignment operator
	CAAudioStream& operator=(const CAAudioStream& rhs) noexcept = default;

	/// Destructor
	virtual ~CAAudioStream() = default;

	/// Move constructor
	CAAudioStream(CAAudioStream&& rhs) noexcept = default;

	/// Move assignment operator
	CAAudioStream& operator=(CAAudioStream&& rhs) noexcept = default;


	/// Creates a @c CAAudioStream with the specified objectID
	CAAudioStream(AudioObjectID objectID) noexcept
	: CAAudioObject{objectID}
	{}

	bool IsActive() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyIsActive)) != 0;
	}

	UInt32 Direction() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyDirection));
	}

	UInt32 TerminalType() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyTerminalType));
	}

	UInt32 StartingChannel() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyStartingChannel));
	}

	UInt32 Latency() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioStreamPropertyLatency));
	}

	CAStreamBasicDescription VirtualFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(CAPropertyAddress(kAudioStreamPropertyVirtualFormat));
	}

	std::vector<AudioStreamRangedDescription> AvailableVirtualFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(CAPropertyAddress(kAudioStreamPropertyAvailableVirtualFormats));
	}

	CAStreamBasicDescription PhysicalFormat() const
	{
		return StructProperty<AudioStreamBasicDescription>(CAPropertyAddress(kAudioStreamPropertyPhysicalFormat));
	}

	std::vector<AudioStreamRangedDescription> AvailablePhysicalFormats() const
	{
		return ArrayProperty<AudioStreamRangedDescription>(CAPropertyAddress(kAudioStreamPropertyAvailablePhysicalFormats));
	}

};

} // namespace SFB
