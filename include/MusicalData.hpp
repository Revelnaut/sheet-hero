#pragma once

enum class PitchClass {
	Null,
	C,
	D,
	E,
	F,
	G,
	A,
	B,
};

enum class Accidental {
	Null,
	Flat,
	Natural,
	Sharp,
};

enum class Value {
	Null,
	Whole,
	Half,
	Quarter,
	Eight,
};

enum class Key {
	Null,
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

enum class NoteState {
	Null,
	Normal,
	Correct,
	Incorrect,
	Active,
};

enum class MusicalGlyph {
	Null = 0,

	Brace = 0xF402,
	ClefG = 0xE050,
	ClefF = 0xE062,

	TimeSignature0 = 0xE080,
	TimeSignature1 = 0xE081,
	TimeSignature2 = 0xE082,
	TimeSignature3 = 0xE083,
	TimeSignature4 = 0xE084,
	TimeSignature5 = 0xE085,
	TimeSignature6 = 0xE086,
	TimeSignature7 = 0xE087,
	TimeSignature8 = 0xE088,
	TimeSignature9 = 0xE089,

	AccidentalFlat = 0xE260,
	AccidentalNatural = 0xE261,
	AccidentalSharp = 0xE262,

	AccidentalFlatSmall = 0xF427,
	AccidentalNaturalSmall = 0xF428,
	AccidentalSharpSmall = 0xF429,

	NoteheadWhole = 0xE0A2,
	NoteheadHalf = 0xE0A3,
	NoteheadBlack = 0xE0A4,

	Flag8thUp = 0xE240,
	Flag8thDown = 0xE241,
};

class DataUtility {
public:
	static int accidental_count_in_key(const Key& key);
	static bool is_key_natural(const Key& key);
	static bool is_key_sharp(const Key& key);
	static bool is_key_flat(const Key& key);
	static MusicalGlyph int_to_time_signature_glyph(int index);
	static MusicalGlyph value_to_notehead_glyph(Value value);
	static Accidental int_to_accidental(int accidental);
	static int value_to_tick(Value value);
};