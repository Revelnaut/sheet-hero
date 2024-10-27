#pragma once

#include "NoteGroup.hpp"
#include "TimeSignature.hpp"
#include <vector>

class Staff {
public:
	Staff();
	Staff(const Staff& source);
	~Staff();

	Staff& operator=(const Staff& source);

	void add_note_group(const NoteGroup& note_group);
	std::vector<NoteGroup> const& get_note_groups() const;

	float get_whole_note_count() const;
	size_t get_note_group_count() const;

	const NoteGroup& note_group_at(float index) const;
	const NoteGroup& note_group_at(int index, int resolution) const;
private:
	std::vector<NoteGroup> m_note_groups{};
};