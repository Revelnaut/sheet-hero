#include "DataTypes.hpp"

int DataUtility::accidentals_in_key(const Key& key)
{
	switch (key) {
	case Key::CMajor:
	case Key::AMinor:
		return 0;
	case Key::GMajor:
	case Key::EMinor:
	case Key::FMajor:
	case Key::DMinor:
		return 1;
	case Key::DMajor:
	case Key::BMinor:
	case Key::BFlatMajor:
	case Key::GMinor:
		return 2;
	case Key::AMajor:
	case Key::FSharpMinor:
	case Key::EFlatMajor:
	case Key::CMinor:
		return 3;
	case Key::EMajor:
	case Key::CSharpMinor:
	case Key::AFlatMajor:
	case Key::FMinor:
		return 4;
	case Key::BMajor:
	case Key::GSharpMinor:
	case Key::DFlatMajor:
	case Key::BFlatMinor:
		return 5;
	case Key::FSharpMajor:
	case Key::DSharpMinor:
	case Key::GFlatMajor:
	case Key::EFlatMinor:
		return 6;
	case Key::CSharpMajor:
	case Key::ASharpMinor:
	case Key::CFlatMajor:
	case Key::AFlatMinor:
		return 7;
	}
	return 0;
}

bool DataUtility::is_key_natural(const Key& key)
{
	switch (key) {
	case Key::CMajor:
	case Key::AMinor:
		return true;
	}
	return false;
}

bool DataUtility::is_key_sharp(const Key& key)
{
	switch (key) {
	case Key::GMajor:
	case Key::EMinor:
	case Key::DMajor:
	case Key::BMinor:
	case Key::AMajor:
	case Key::FSharpMinor:
	case Key::EMajor:
	case Key::CSharpMinor:
	case Key::BMajor:
	case Key::GSharpMinor:
	case Key::FSharpMajor:
	case Key::DSharpMinor:
	case Key::CSharpMajor:
	case Key::ASharpMinor:
		return true;
	}
	return false;
}

bool DataUtility::is_key_flat(const Key& key)
{
	return !is_key_natural(key) && !is_key_sharp(key);
}

MusicalGlyph DataUtility::int_to_time_signature_glyph(int index)
{
	switch (index) {
	case 0: return MusicalGlyph::TimeSignature0;
	case 1: return MusicalGlyph::TimeSignature1;
	case 2: return MusicalGlyph::TimeSignature2;
	case 3: return MusicalGlyph::TimeSignature3;
	case 4: return MusicalGlyph::TimeSignature4;
	case 5: return MusicalGlyph::TimeSignature5;
	case 6: return MusicalGlyph::TimeSignature6;
	case 7: return MusicalGlyph::TimeSignature7;
	case 8: return MusicalGlyph::TimeSignature8;
	case 9: return MusicalGlyph::TimeSignature9;
	}
	return MusicalGlyph::Null;
}
