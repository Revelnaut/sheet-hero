#include "Note.hpp"

Note::Note() {}

Note::Note(PitchClass pitch_class, Accidental accidental, int octave)
	: m_pitch_class{ pitch_class }, m_accidental{ accidental }, m_octave{ octave } {}

Note::Note(const Note& source)
	: m_pitch_class{ source.m_pitch_class }, m_accidental{ source.m_accidental }, m_octave{ source.m_octave } {}

Note::~Note() {}

Note& Note::operator=(const Note& source) {
	m_pitch_class = source.m_pitch_class;
	m_accidental = source.m_accidental;
	m_octave = source.m_octave;

	return *this;
}

PitchClass Note::get_pitch_class() const {
	return m_pitch_class;
}

Accidental Note::get_accidental() const {
	return m_accidental;
}

int Note::get_octave() const {
	return m_octave;
}

int Note::get_midi_pitch() const {
	return ((m_octave + 1) * 12) + get_pitch_relative_to_octave();
}

int Note::get_pitch_relative_to_octave() const {
	int accidental_value{ 0 };
	int pitch_class_value{ 0 };

	switch (m_pitch_class) {
	case PitchClass::C: pitch_class_value = 0; break;
	case PitchClass::D: pitch_class_value = 2; break;
	case PitchClass::E: pitch_class_value = 4; break;
	case PitchClass::F: pitch_class_value = 5; break;
	case PitchClass::G: pitch_class_value = 7; break;
	case PitchClass::A: pitch_class_value = 9; break;
	case PitchClass::B: pitch_class_value = 11; break;
	}

	switch (m_accidental) {
	case Accidental::Flat: accidental_value = -1; break;
	case Accidental::Natural: accidental_value = 0; break;
	case Accidental::Sharp: accidental_value = 1; break;
	}

	return pitch_class_value + accidental_value;
}

int Note::get_staff_position() const {
	int position{ 0 };

	switch (m_pitch_class) {
	case PitchClass::C: position = 0; break;
	case PitchClass::D: position = 1; break;
	case PitchClass::E: position = 2; break;
	case PitchClass::F: position = 3; break;
	case PitchClass::G: position = 4; break;
	case PitchClass::A: position = 5; break;
	case PitchClass::B: position = 6; break;
	}

	position += (m_octave - 4) * 7;

	return position;
}