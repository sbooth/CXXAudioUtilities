# CXXAudioUtilities

Portions of this repository have been split into separate packages:

| Repository | Description |
| --- | --- |
| [CXXCoreAudio](https://github.com/sbooth/CXXCoreAudio) | Assorted C++ classes simplifying common operations in Core Audio. |
| [CXXAudioToolbox](https://github.com/sbooth/CXXAudioToolbox) | Assorted C++ classes simplifying the use of Audio Toolbox. |
| [CXXAudioHardware](https://github.com/sbooth/CXXAudioHardware) | Assorted C++ classes simplifying the use of Audio Hardware. |
| [CXXRingBuffer](https://github.com/sbooth/CXXRingBuffer) | A lock-free SPSC ring buffer. |
| [CXXDispatchSemaphore](https://github.com/sbooth/CXXDispatchSemaphore) | A [`dispatch_semaphore_t`](https://developer.apple.com/documentation/dispatch/dispatch-semaphore?language=objc) wrapper. |
| [CXXOSUnfairLock](https://github.com/sbooth/CXXOSUnfairLock) | An [`os_unfair_lock`](https://developer.apple.com/documentation/os/1646466-os_unfair_lock_lock) wrapper satisfying the [`Lockable`](https://en.cppreference.com/w/cpp/named_req/Lockable) requirements. |

Assorted C++ classes simplifying common operations in Core Audio and Audio Toolbox.

## Installation

### Swift Package Manager

Add a package dependency to https://github.com/sbooth/CXXAudioUtilities in Xcode.

### Manual or Custom Build

1. Clone the [CXXAudioUtilities](https://github.com/sbooth/CXXAudioUtilities) repository.
2. `swift build`.

## CXXAudioUtilities

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
