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

	void add_note_group(const NoteGroup& note_group);
	std::vector<NoteGroup> const& get_note_groups() const;

	size_t get_note_group_count() const;

	int free_space_in_eights(const TimeSignature& time_signature) const;
private:
	std::vector<NoteGroup> m_note_groups{};
};