// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
	name: "CXXAudioUtilities",
	products: [
		.library(
			name: "CXXAudioUtilities",
			targets: [
				"CXXAudioUtilities",
			]),
	],
	targets: [
		.target(
			name: "CXXAudioUtilities",
			linkerSettings: [
				.linkedFramework("CoreAudio"),
				.linkedFramework("AudioToolbox"),
			]),
		.testTarget(
			name: "CXXAudioUtilitiesTests",
			dependencies: [
				"CXXAudioUtilities",
			]),
	],
	cxxLanguageStandard: .cxx17
)
