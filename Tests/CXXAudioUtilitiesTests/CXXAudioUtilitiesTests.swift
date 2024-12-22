import XCTest
#if swift(>=5.9)
@testable import CXXAudioUtilities
#endif

final class CXXAudioUtilitiesTests: XCTestCase {
#if swift(>=5.9)
	func testByteStream() {
		let bs = SFB.ByteStream()
		XCTAssertEqual(bs.Length(), 0)
		XCTAssertEqual(bs.Remaining(), 0)
		XCTAssertEqual(bs.Position(), 0)
	}

	func testStreamDescription() {
		let asbd = SFB.CAStreamBasicDescription(.float32, 44100, 2, false)
		XCTAssertTrue(asbd.IsPCM())
		XCTAssertTrue(asbd.IsFloat())
		XCTAssertFalse(asbd.IsInteger())
		XCTAssertEqual(asbd.mSampleRate, 44100)
		XCTAssertEqual(asbd.ChannelCount(), 2)
		XCTAssertFalse(asbd.IsInterleaved())
		XCTAssertTrue(asbd.IsNonInterleaved())
	}

	func testChannelLayout() {
		let stereo = SFB.CAChannelLayout.Stereo
		XCTAssertEqual(stereo.ChannelCount(), 2)
	}
#endif
}
