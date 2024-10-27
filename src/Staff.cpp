#include "Staff.hpp"


Staff::Staff() {}

Staff::Staff(const Staff& source)
	: m_note_groups{ source.m_note_groups } {}

Staff::~Staff() {}

Staff& Staff::operator=(const Staff& source) {
	m_note_groups = source.m_note_groups;

	return *this;
}

void Staff::add_note_group(const NoteGroup& note_group) {
	m_note_groups.push_back(note_group);
}

std::vector<NoteGroup> const& Staff::get_note_groups() const {
	return m_note_groups;
}

// TODO: Test!
float Staff::get_whole_note_count() const {
	float count{};

	for ( auto& note_group : m_note_groups ) {
		count += DataUtility::value_relative_size(note_group.get_value());
	}

	return count;
}

size_t Staff::get_note_group_count() const {
	return m_note_groups.size();
}

// TODO: Test!
const NoteGroup& Staff::note_group_at(float index) const {
	float position = index * get_whole_note_count();
	float count{};
	for ( auto& note_group : m_note_groups ) {
		count += DataUtility::value_relative_size(note_group.get_value());
		if ( count > position ) {
			return note_group;
		}
	}
	return m_note_groups.at(0);
}

const NoteGroup& Staff::note_group_at(int index, int resolution) const {
	// TODO: Return actual value
	return m_note_groups.back();
}
