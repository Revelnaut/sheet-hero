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
