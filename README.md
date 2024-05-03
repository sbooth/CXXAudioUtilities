# CXXAudioUtilities

Assorted C++ classes simplifying common operations in Core Audio and Audio Toolbox.

> [!NOTE]
> C++17 is required.

## Installation

### Swift Package Manager

Add a package dependency to https://github.com/sbooth/CXXAudioUtilities in Xcode.

### Manual or Custom Build

1. Clone the [CXXAudioUtilities](https://github.com/sbooth/CXXAudioUtilities) repository.
2. `swift build`.

## CXXAudioUtilities

### CoreAudioTypes Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::CABufferList](Sources/CXXAudioUtilities/include/SFBCABufferList.hpp) | A class wrapping a Core Audio `AudioBufferList` with a specific format, frame capacity, and frame length |
| [SFB::CAChannelLayout](Sources/CXXAudioUtilities/include/SFBCAChannelLayout.hpp) | A class wrapping a Core Audio `AudioChannelLayout` |
| [SFB::CAStreamBasicDescription](Sources/CXXAudioUtilities/include/SFBCAStreamBasicDescription.hpp) | A class extending the functionality of a Core Audio `AudioStreamBasicDescription` |
| [SFB::CATimeStamp](Sources/CXXAudioUtilities/include/SFBCATimeStamp.hpp) | A class extending the functionality of a Core Audio `AudioTimeStamp` |
| [SFB::CAException](Sources/CXXAudioUtilities/include/SFBCAException.hpp) | `std::error_category` for handling Core Audio errors as exceptions |

#### AudioHardware Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::CAPropertyAddress](Sources/CXXAudioUtilities/include/SFBCAPropertyAddress.hpp) | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| [SFB::CAAudioObject](Sources/CXXAudioUtilities/include/SFBCAAudioObject.hpp) | A wrapper around a HAL audio object |
| [SFB::CAAudioDevice](Sources/CXXAudioUtilities/include/SFBCAAudioDevice.hpp) | A wrapper around a HAL audio device |
| [SFB::CAAudioStream](Sources/CXXAudioUtilities/include/SFBCAAudioStream.hpp) | A wrapper around a HAL audio stream |
| [SFB::CAAudioSystemObject](Sources/CXXAudioUtilities/include/SFBCAAudioSystemObject.hpp) | A wrapper around `kAudioObjectSystemObject` |

### AudioToolbox Wrappers

| C++ Class | Description |
| --- | --- |
| [SFB::AudioFileWrapper](Sources/CXXAudioUtilities/include/SFBAudioFileWrapper.hpp) | A bare-bones wrapper around `AudioFile` modeled after `std::unique_ptr` |
| [SFB::ExtAudioFileWrapper](Sources/CXXAudioUtilities/include/SFBExtAudioFileWrapper.hpp) | A bare-bones wrapper around `ExtAudioFile` modeled after `std::unique_ptr` |
| [SFB::CAAUGraph](Sources/CXXAudioUtilities/include/SFBCAAUGraph.hpp) | A wrapper around `AUGraph` |
| [SFB::CAAudioFile](Sources/CXXAudioUtilities/include/SFBCAAudioFile.hpp) | A wrapper around `AudioFile` |
| [SFB::CAExtAudioFile](Sources/CXXAudioUtilities/include/SFBCAExtAudioFile.hpp) | A wrapper around `ExtAudioFile` |
| [SFB::CAAudioFormat](Sources/CXXAudioUtilities/include/SFBCAAudioFormat.hpp) | A wrapper around `AudioFormat` |
| [SFB::CAAudioConverter](Sources/CXXAudioUtilities/include/SFBCAAudioConverter.hpp) | A wrapper around `AudioConverter` |

### Ring Buffers

| C++ Class | Description |
| --- | --- |
| [SFB::RingBuffer](Sources/CXXAudioUtilities/include/SFBRingBuffer.hpp) | A generic ring buffer |
| [SFB::AudioRingBuffer](Sources/CXXAudioUtilities/include/SFBAudioRingBuffer.hpp) | A ring buffer supporting non-interleaved audio |
| [SFB::CARingBuffer](Sources/CXXAudioUtilities/include/SFBCARingBuffer.hpp) | A ring buffer supporting timestamped non-interleaved audio |

### Utility Classes

| C++ Class | Description |
| --- | --- |
| [SFB::ByteStream](Sources/CXXAudioUtilities/include/SFBByteStream.hpp) | A `ByteStream` provides heterogeneous typed access to an untyped buffer |
| [SFB::CFWrapper](Sources/CXXAudioUtilities/include/SFBCFWrapper.hpp) | A wrapper around a Core Foundation object |
| [SFB::DeferredClosure](Sources/CXXAudioUtilities/include/SFBDeferredClosure.hpp) | A class that calls a closure upon destruction |
| [SFB::DispatchSemaphore](Sources/CXXAudioUtilities/include/SFBDispatchSemaphore.hpp) | A wrapper around `dispatch_semaphore_t` |
| [SFB::UnfairLock](Sources/CXXAudioUtilities/include/SFBUnfairLock.hpp) | A wrapper around `os_unfair_lock` implementing C++ `Lockable` |

| C++ Class | Description |
| --- | --- |
| [SFB::AudioUnitRecorder](Sources/CXXAudioUtilities/include/SFBAudioUnitRecorder.hpp) | A class that asynchronously writes the output from an `AudioUnit` to a file |

## License

Released under the [MIT License](https://github.com/sbooth/CXXAudioUtilities/blob/main/LICENSE.txt).
