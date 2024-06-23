#include "Note.hpp"
#include <vector>
#include <iostream>

unsigned int Note::m_id_seed{ 0 };

Note::Note() {
	m_id = ++m_id_seed;
}

Note::Note(PitchClass pitch_class, Accidental accidental, int octave)
	: m_pitch_class{ pitch_class },
	m_accidental{ accidental },
	m_octave{ octave } {
	m_id = ++m_id_seed;
}

Note::Note(const Note& source)
	: m_pitch_class{ source.m_pitch_class },
	m_accidental{ source.m_accidental },
	m_octave{ source.m_octave } {
	m_id = ++m_id_seed;
}

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
	return ( ( m_octave + 1 ) * 12 ) + get_pitch_relative_to_octave();
}

int Note::get_pitch_relative_to_octave() const {
	int accidental_value{ 0 };
	int pitch_class_value{ 0 };

	switch ( m_pitch_class ) {
	case PitchClass::C: pitch_class_value = 0; break;
	case PitchClass::D: pitch_class_value = 2; break;
	case PitchClass::E: pitch_class_value = 4; break;
	case PitchClass::F: pitch_class_value = 5; break;
	case PitchClass::G: pitch_class_value = 7; break;
	case PitchClass::A: pitch_class_value = 9; break;
	case PitchClass::B: pitch_class_value = 11; break;
	}

	switch ( m_accidental ) {
	case Accidental::Flat: accidental_value = -1; break;
	case Accidental::Natural: accidental_value = 0; break;
	case Accidental::Sharp: accidental_value = 1; break;
	}

	return pitch_class_value + accidental_value;
}

int Note::get_staff_position() const {
	int position{ 0 };

	switch ( m_pitch_class ) {
	case PitchClass::C: position = 0; break;
	case PitchClass::D: position = 1; break;
	case PitchClass::E: position = 2; break;
	case PitchClass::F: position = 3; break;
	case PitchClass::G: position = 4; break;
	case PitchClass::A: position = 5; break;
	case PitchClass::B: position = 6; break;
	}

	position += ( m_octave - 4 ) * 7;

	return position;
}

unsigned int Note::get_id() const {
	return m_id;
}

std::string Note::to_string() const {
	std::string return_value;
	switch ( m_pitch_class ) {
	case PitchClass::C: return_value += "C"; break;
	case PitchClass::D: return_value += "D"; break;
	case PitchClass::E: return_value += "E"; break;
	case PitchClass::F: return_value += "F"; break;
	case PitchClass::G: return_value += "G"; break;
	case PitchClass::A: return_value += "A"; break;
	case PitchClass::B: return_value += "B"; break;
	}
	switch ( m_accidental ) {
	case Accidental::Flat: return_value += "b"; break;
	case Accidental::Sharp: return_value += "#"; break;
	}
	return_value += std::to_string(m_octave);
	return return_value;
}

bool operator==(const Note& note_1, const Note& note_2) {
	return note_1.m_accidental == note_2.m_accidental
		&& note_1.m_octave == note_2.m_octave
		&& note_1.m_pitch_class == note_2.m_pitch_class;
}

bool operator!=(const Note& note_1, const Note& note_2) {
	return !(operator==(note_1, note_2));
}

bool operator<(const Note& note_1, const Note& note_2) {
	return note_1.get_midi_pitch() < note_2.get_midi_pitch();
}

bool operator>(const Note& note_1, const Note& note_2) {
	return operator<(note_2, note_1);
}

bool operator<=(const Note& note_1, const Note& note_2) {
	return !( operator>(note_1, note_2) );
}

bool operator>=(const Note& note_1, const Note& note_2) {
	return !( operator<(note_1, note_2) );
}
