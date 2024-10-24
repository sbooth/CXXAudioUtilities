// swift-tools-version: 5.9
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
			swiftSettings: [
				.interoperabilityMode(.Cxx),
			],
			linkerSettings: [
				.linkedFramework("CoreAudio"),
				.linkedFramework("AudioToolbox"),
			]),
		.testTarget(
			name: "CXXAudioUtilitiesTests",
			dependencies: [
				"CXXAudioUtilities",
			],
			swiftSettings: [
				.interoperabilityMode(.Cxx),
			]),
	],
	cxxLanguageStandard: .cxx17
)
