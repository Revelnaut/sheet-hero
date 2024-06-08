#pragma once

enum class PitchClass {
	C,
	D,
	E,
	F,
	G,
	A,
	B,
};

enum class Accidental {
	Flat,
	Natural,
	Sharp,
};

enum class Value {
	Zero,
	Whole,
	Half,
	Quarter,
	Eight,
};

enum class Key {
	CMajor,

	GMajor,
	DMajor,
	AMajor,
	EMajor,
	BMajor,
	FSharpMajor,
	CSharpMajor,

	FMajor,
	BFlatMajor,
	EFlatMajor,
	AFlatMajor,
	DFlatMajor,
	GFlatMajor,
	CFlatMajor,

	AMinor,

	EMinor,
	BMinor,
	FSharpMinor,
	CSharpMinor,
	GSharpMinor,
	DSharpMinor,
	ASharpMinor,

	DMinor,
	GMinor,
	CMinor,
	FMinor,
	BFlatMinor,
	EFlatMinor,
	AFlatMinor,
};