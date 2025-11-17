# CXXAudioUtilities

Assorted C++ classes simplifying common operations in Core Audio and Audio Toolbox.

Portions of this repository have been split into separate packages:

| Repository | Description |
| --- | --- |
| [CXXCoreAudio](https://github.com/sbooth/CXXCoreAudio) | Assorted C++ classes simplifying common operations in Core Audio. |
| [CXXAudioToolbox](https://github.com/sbooth/CXXAudioToolbox) | Assorted C++ classes simplifying the use of Audio Toolbox. |
| [CXXRingBuffer](https://github.com/sbooth/CXXRingBuffer) | A lock-free SPSC ring buffer. |
| [CXXDispatchSemaphore](https://github.com/sbooth/CXXDispatchSemaphore) | A [`dispatch_semaphore_t`](https://developer.apple.com/documentation/dispatch/dispatch-semaphore?language=objc) wrapper. |
| [CXXOSUnfairLock](https://github.com/sbooth/CXXOSUnfairLock) | An [`os_unfair_lock`](https://developer.apple.com/documentation/os/1646466-os_unfair_lock_lock) wrapper satisfying the [`Lockable`](https://en.cppreference.com/w/cpp/named_req/Lockable) requirements. |

## Installation

### Swift Package Manager

Add a package dependency to https://github.com/sbooth/CXXAudioUtilities in Xcode.

### Manual or Custom Build

1. Clone the [CXXAudioUtilities](https://github.com/sbooth/CXXAudioUtilities) repository.
2. `swift build`.

## CXXAudioUtilities

### AudioHardware Wrappers

| Class | Description |
| --- | --- |
| [SFB::CAPropertyAddress](Sources/CXXAudioUtilities/include/SFBCAPropertyAddress.hpp) | A class extending the functionality of a Core Audio `AudioObjectPropertyAddress` |
| [SFB::CAAudioObject](Sources/CXXAudioUtilities/include/SFBCAAudioObject.hpp) | A wrapper around a HAL audio object |
| [SFB::CAAudioDevice](Sources/CXXAudioUtilities/include/SFBCAAudioDevice.hpp) | A wrapper around a HAL audio device |
| [SFB::CAAudioStream](Sources/CXXAudioUtilities/include/SFBCAAudioStream.hpp) | A wrapper around a HAL audio stream |
| [SFB::CAAudioSystemObject](Sources/CXXAudioUtilities/include/SFBCAAudioSystemObject.hpp) | A wrapper around `kAudioObjectSystemObject` |

### Ring Buffers

| Class | Description |
| --- | --- |
| [SFB::CARingBuffer](Sources/CXXAudioUtilities/include/SFBCARingBuffer.hpp) | A ring buffer supporting timestamped non-interleaved audio |

### Utility Classes

| Class | Description |
| --- | --- |
| [SFB::ByteStream](Sources/CXXAudioUtilities/include/SFBByteStream.hpp) | A `ByteStream` provides heterogeneous typed access to an untyped buffer |
| [SFB::CFWrapper](Sources/CXXAudioUtilities/include/SFBCFWrapper.hpp) | A wrapper around a Core Foundation object |
| [SFB::ScopeGuard](Sources/CXXAudioUtilities/include/SFBScopeGuard.hpp) | A class that calls a function upon destruction |

| Class | Description |
| --- | --- |
| [SFB::AudioUnitRecorder](Sources/CXXAudioUtilities/include/SFBAudioUnitRecorder.hpp) | A class that asynchronously writes the output from an `AudioUnit` to a file |

## License

Released under the [MIT License](https://github.com/sbooth/CXXAudioUtilities/blob/main/LICENSE.txt).
