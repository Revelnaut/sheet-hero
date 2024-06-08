#pragma once
#include "DataTypes.hpp"

class Note {
public:
	Note();
	Note(PitchClass pitch_class, Accidental accidental, int octave);
	Note(const Note& source);
	~Note();

	Note& operator=(const Note& source);

	PitchClass get_pitch_class() const;
	Accidental get_accidental() const;
	int get_octave() const;
	int get_midi_pitch() const;
	int get_pitch_relative_to_octave() const;
	int get_staff_position() const;

	void set_state(NoteState state);
	NoteState get_state() const;
private:
	PitchClass m_pitch_class{ PitchClass::C };
	Accidental m_accidental{ Accidental::Natural };
	int m_octave{ 4 };
	NoteState m_state{};
};
