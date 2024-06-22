#pragma once
#include "MusicalData.hpp"

class Note {
public:
	Note();
	Note(PitchClass pitch_class, Accidental accidental, int octave);
	Note(const Note& source);
	~Note();

	Note& operator=(const Note& source);

	friend bool operator==(const Note& note_1, const Note& note_2);
	friend bool operator!=(const Note& note_1, const Note& note_2);

	friend bool operator<(const Note& note_1, const Note& note_2);
	friend bool operator>(const Note& note_1, const Note& note_2);

	friend bool operator<=(const Note& note_1, const Note& note_2);
	friend bool operator>=(const Note& note_1, const Note& note_2);

	PitchClass get_pitch_class() const;
	Accidental get_accidental() const;
	int get_octave() const;
	int get_midi_pitch() const;
	int get_pitch_relative_to_octave() const;
	int get_staff_position() const;

	int get_id() const;
private:
	PitchClass m_pitch_class{ PitchClass::C };
	Accidental m_accidental{ Accidental::Null };
	int m_octave{ 4 };
	unsigned int m_id{};

	static unsigned int m_id_seed;
};
