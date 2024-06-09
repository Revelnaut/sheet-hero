#include "NoteGroup.hpp"
#include <algorithm>

NoteGroup::NoteGroup() {}

NoteGroup::NoteGroup(Value value)
	: m_value{ value } {}

NoteGroup::NoteGroup(const NoteGroup& source)
	: m_notes{ source.m_notes }, m_value{ source.m_value } {}

NoteGroup::~NoteGroup() {}

NoteGroup& NoteGroup::operator=(const NoteGroup& source) {
	m_notes = source.m_notes;
	m_value = source.m_value;

	return *this;
}

void NoteGroup::add_note(const Note& note) {
	m_notes.push_back(note);
	std::sort(m_notes.begin(), m_notes.end(), [](const Note& left, const Note& right) {
		return left.get_staff_position() < right.get_staff_position();
	});
}

void NoteGroup::set_value(Value value) {
	m_value = value;
}

std::vector<Note> const& NoteGroup::get_notes() const {
	return m_notes;
}

Value const& NoteGroup::get_value() const {
	return m_value;
}

bool NoteGroup::is_rest() const {
	return m_notes.size() == 0;
}