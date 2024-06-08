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

	void add_note_set(const NoteSet& note_set);
	void set_time_signature(const TimeSignature & time_signature);
	
	TimeSignature const& get_time_signature() const;
	NoteSet const& get_note_set(int index) const;
	size_t get_note_set_count() const;
	std::vector<NoteSet> const& get_note_sets() const;
private:
	std::vector<NoteSet> m_sets{};
	TimeSignature m_time_signature{};
};