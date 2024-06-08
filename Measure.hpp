#pragma once

#include "NoteSet.hpp"
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
	
	void add_treble_note_set(const NoteSet& note_set);
	std::vector<NoteSet> const& get_treble_note_sets() const;

	void add_bass_note_set(const NoteSet& note_set);
	std::vector<NoteSet> const& get_bass_note_sets() const;

	size_t get_note_set_count() const;
private:
	std::vector<NoteSet> m_treble_sets{};
	std::vector<NoteSet> m_bass_sets{};
	TimeSignature m_time_signature{};
};