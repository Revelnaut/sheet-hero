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

static Accidental int_to_accidental(int accidental) {
	switch (accidental) {
	case -1: return Accidental::Flat;
	case 0: return Accidental::Natural;
	case 1: return Accidental::Sharp;
	}
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
	case Key::GMajor: create("G Major", 0, 0, 0, 1, 0, 0, 0); break;
	}
}