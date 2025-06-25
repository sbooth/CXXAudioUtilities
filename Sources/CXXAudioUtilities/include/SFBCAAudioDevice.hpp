//
// Copyright © 2021-2025 Stephen F. Booth <me@sbooth.org>
// Part of https://github.com/sbooth/CXXAudioUtilities
// MIT license
//

#pragma once

#import "SFBCAAudioObject.hpp"
#import "SFBCAAudioStream.hpp"

namespace SFB {

class CAAudioDevice : public CAAudioObject
{

public:

	/// Creates an unknown @c SFBCAAudioObject
	constexpr CAAudioDevice() noexcept = default;

	/// Copy constructor
	CAAudioDevice(const CAAudioDevice& rhs) noexcept = default;

	/// Assignment operator
	CAAudioDevice& operator=(const CAAudioDevice& rhs) noexcept = default;

	/// Destructor
	virtual ~CAAudioDevice() = default;

	/// Move constructor
	CAAudioDevice(CAAudioDevice&& rhs) noexcept = default;

	/// Move assignment operator
	CAAudioDevice& operator=(CAAudioDevice&& rhs) noexcept = default;


	/// Creates a @c CAAudioDevice with the specified objectID
	CAAudioDevice(AudioObjectID objectID) noexcept
	: CAAudioObject{objectID}
	{}


	//	kAudioDevicePropertyConfigurationApplication        = 'capp',

	CFString UID() const
	{
		return CFTypeProperty<CFStringRef>(CAPropertyAddress(kAudioDevicePropertyDeviceUID));
	}

	CFString ModelUID() const
	{
		return CFTypeProperty<CFStringRef>(CAPropertyAddress(kAudioDevicePropertyModelUID));
	}

	//	kAudioDevicePropertyTransportType                   = 'tran',
	//	kAudioDevicePropertyRelatedDevices                  = 'akin',
	//	kAudioDevicePropertyClockDomain                     = 'clkd',
	//	kAudioDevicePropertyDeviceIsAlive                   = 'livn',
	//	kAudioDevicePropertyDeviceIsRunning                 = 'goin',
	//	kAudioDevicePropertyDeviceCanBeDefaultDevice        = 'dflt',
	//	kAudioDevicePropertyDeviceCanBeDefaultSystemDevice  = 'sflt',

	UInt32 Latency(CAAudioObjectDirectionalScope scope) const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioDevicePropertyLatency, scope == CAAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	std::vector<AudioObjectID> StreamIDs(CAAudioObjectDirectionalScope scope) const
	{
		return ArrayProperty<AudioObjectID>(CAPropertyAddress(kAudioDevicePropertyStreams, scope == CAAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	std::vector<CAAudioStream> Streams(CAAudioObjectDirectionalScope scope) const
	{
		auto vec = StreamIDs(scope);
		std::vector<CAAudioStream> result(vec.size());
		std::transform(vec.cbegin(), vec.cend(), result.begin(), [](AudioObjectID objectID) { return CAAudioStream(objectID); });
		return result;
	}

	//	kAudioObjectPropertyControlList                     = 'ctrl',

	UInt32 SafetyOffset(CAAudioObjectDirectionalScope scope) const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioDevicePropertySafetyOffset, scope == CAAudioObjectDirectionalScope::input ? kAudioObjectPropertyScopeInput : kAudioObjectPropertyScopeOutput));
	}

	Float64 NominalSampleRate() const
	{
		return ArithmeticProperty<Float64>(CAPropertyAddress(kAudioDevicePropertyNominalSampleRate));
	}

	//	kAudioDevicePropertyAvailableNominalSampleRates     = 'nsr#',
	//	kAudioDevicePropertyIcon                            = 'icon',
	//	kAudioDevicePropertyIsHidden                        = 'hidn',
	//	kAudioDevicePropertyPreferredChannelsForStereo      = 'dch2',
	//	kAudioDevicePropertyPreferredChannelLayout          = 'srnd'

	//	kAudioDevicePropertyPlugIn                          = 'plug',
	//	kAudioDevicePropertyDeviceHasChanged                = 'diff',
	//	kAudioDevicePropertyDeviceIsRunningSomewhere        = 'gone',
	//	kAudioDeviceProcessorOverload                       = 'over',
	//	kAudioDevicePropertyIOStoppedAbnormally             = 'stpd',
	//	kAudioDevicePropertyHogMode                         = 'oink',

	UInt32 BufferFrameSize() const
	{
		return ArithmeticProperty<UInt32>(CAPropertyAddress(kAudioDevicePropertyBufferFrameSize));
	}

	//	kAudioDevicePropertyBufferFrameSizeRange            = 'fsz#',
	//	kAudioDevicePropertyUsesVariableBufferFrameSizes    = 'vfsz',
	//	kAudioDevicePropertyIOCycleUsage                    = 'ncyc',
	//	kAudioDevicePropertyStreamConfiguration             = 'slay',
	//	kAudioDevicePropertyIOProcStreamUsage               = 'suse',
	//	kAudioDevicePropertyActualSampleRate                = 'asrt',
	//	kAudioDevicePropertyClockDevice                     = 'apcd',
	//	kAudioDevicePropertyIOThreadOSWorkgroup				= 'oswg'

};

} /* namespace SFB */
