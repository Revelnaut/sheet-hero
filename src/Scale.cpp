#include "Scale.hpp"

Scale::Scale()
{
	set_key(Key::CMajor);
}

Scale::Scale(Key key)
{
	set_key(key);
}

Scale::Scale(std::string name, int c, int d, int e, int f, int g, int a, int b)
{
	create(name, c, d, e, f, g, a, b);
}

Scale::Scale(const Scale& source)
{
	m_name = source.m_name;
	m_accidentals = source.m_accidentals;
}

Scale& Scale::operator=(const Scale& source)
{
	m_name = source.m_name;
	m_accidentals = source.m_accidentals;

	return *this;
}

bool Scale::is_sharp(const PitchClass& pitch_class) const {
	return m_accidentals.at(pitch_class) == Accidental::Sharp;
}

bool Scale::is_flat(const PitchClass& pitch_class) const {
	return m_accidentals.at(pitch_class) == Accidental::Flat;
}

bool Scale::is_natural(const PitchClass& pitch_class) const {
	return m_accidentals.at(pitch_class) == Accidental::Natural;
}

void Scale::set_accidental(const PitchClass& pitch_class, const Accidental& accidental) {
	m_accidentals[pitch_class] = accidental;
}

const Accidental& Scale::get_accidental(const PitchClass& pitch_class) const
{
	return m_accidentals.at(pitch_class);
}

static Accidental int_to_accidental(int accidental) {
	switch (accidental) {
	case -1: return Accidental::Flat;
	case 0: return Accidental::Natural;
	case 1: return Accidental::Sharp;
	}
	return Accidental::Natural;
}

void Scale::set_accidentals(int c, int d, int e, int f, int g, int a, int b)
{
	m_accidentals[PitchClass::C] = int_to_accidental(c);
	m_accidentals[PitchClass::D] = int_to_accidental(d);
	m_accidentals[PitchClass::E] = int_to_accidental(e);
	m_accidentals[PitchClass::F] = int_to_accidental(f);
	m_accidentals[PitchClass::G] = int_to_accidental(g);
	m_accidentals[PitchClass::A] = int_to_accidental(a);
	m_accidentals[PitchClass::B] = int_to_accidental(b);
}

void Scale::set_name(const std::string& name)
{
	m_name = name;
}

void Scale::create(std::string name, int c, int d, int e, int f, int g, int a, int b)
{
	set_name(name);
	set_accidentals(c, d, e, f, g, a, b);
}

void Scale::set_key(Key key)
{
	switch (key) {
	case Key::CMajor: create("C Major", 0, 0, 0, 0, 0, 0, 0); break;
	case Key::AMinor: create("A minor", 0, 0, 0, 0, 0, 0, 0); break;

	case Key::GMajor: create("G Major", 0, 0, 0, 1, 0, 0, 0); break;
	case Key::DMajor: create("D Major", 1, 0, 0, 1, 0, 0, 0); break;
	case Key::AMajor: create("A Major", 1, 0, 0, 1, 1, 0, 0); break;
	case Key::EMajor: create("E Major", 1, 1, 0, 1, 1, 0, 0); break;
	case Key::BMajor: create("B Major", 1, 1, 0, 1, 1, 1, 0); break;
	case Key::FSharpMajor: create("F# Major", 1, 1, 1, 1, 1, 1, 0); break;
	case Key::CSharpMajor: create("F# Major", 1, 1, 1, 1, 1, 1, 1); break;

	case Key::EMinor: create("E minor", 0, 0, 0, 1, 0, 0, 0); break;
	case Key::BMinor: create("B minor", 1, 0, 0, 1, 0, 0, 0); break;
	case Key::FSharpMinor: create("F# minor", 1, 0, 0, 1, 1, 0, 0); break;
	case Key::CSharpMinor: create("C# minor", 1, 1, 0, 1, 1, 0, 0); break;
	case Key::GSharpMinor: create("G# minor", 1, 1, 0, 1, 1, 1, 0); break;
	case Key::DSharpMinor: create("D# minor", 1, 1, 1, 1, 1, 1, 0); break;
	case Key::ASharpMinor: create("A# minor", 1, 1, 1, 1, 1, 1, 1); break;

	case Key::FMajor: create("F Major", 0, 0, 0, 0, 0, 0, -1); break;
	case Key::BFlatMajor: create("Bb Major", 0, 0, -1, 0, 0, 0, -1); break;
	case Key::EFlatMajor: create("Eb Major", 0, 0, -1, 0, 0, -1, -1); break;
	case Key::AFlatMajor: create("Ab Major", 0, -1, -1, 0, 0, -1, -1); break;
	case Key::DFlatMajor: create("Db Major", 0, -1, -1, 0, -1, -1, -1); break;
	case Key::GFlatMajor: create("Gb Major", -1, -1, -1, 0, -1, -1, -1); break;
	case Key::CFlatMajor: create("Cb Major", -1, -1, -1, -1, -1, -1, -1); break;

	case Key::DMinor: create("D minor", 0, 0, 0, 0, 0, 0, -1); break;
	case Key::GMinor: create("G minor", 0, 0, -1, 0, 0, 0, -1); break;
	case Key::CMinor: create("C minor", 0, 0, -1, 0, 0, -1, -1); break;
	case Key::FMinor: create("F minor", 0, -1, -1, 0, 0, -1, -1); break;
	case Key::BFlatMinor: create("Bb minor", 0, -1, -1, 0, -1, -1, -1); break;
	case Key::EFlatMinor: create("Eb minor", -1, -1, -1, 0, -1, -1, -1); break;
	case Key::AFlatMinor: create("Ab minor", -1, -1, -1, -1, -1, -1, -1); break;
	
	}
}