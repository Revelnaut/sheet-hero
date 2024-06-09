#pragma once

#include "NoteGroup.hpp"
#include "TimeSignature.hpp"
#include <vector>

class Measure {
public:
	Measure();
	Measure(const Measure& source);
	~Measure();

	Measure& operator=(const Measure& source);

	void set_time_signature(const TimeSignature & time_signature);
	TimeSignature const& get_time_signature() const;
	
	void add_treble_note_group(const NoteGroup& note_group);
	std::vector<NoteGroup> const& get_treble_note_groups() const;

	void add_bass_note_group(const NoteGroup& note_group);
	std::vector<NoteGroup> const& get_bass_note_groups() const;

	size_t get_note_group_count() const;

	int free_treble_space_in_eights() const;
	int free_bass_space_in_eights() const;
private:
	std::vector<NoteGroup> m_treble_note_groups{};
	std::vector<NoteGroup> m_bass_note_groups{};
	TimeSignature m_time_signature{};
};