#include "NoteSet.hpp"
#include <algorithm>

NoteSet::NoteSet() {}

NoteSet::NoteSet(Value value)
	: m_value{ value } {}

NoteSet::NoteSet(const NoteSet& source)
	: m_notes{ source.m_notes }, m_value{ source.m_value } {}

NoteSet::~NoteSet() {}

NoteSet& NoteSet::operator=(const NoteSet& source) {
	m_notes = source.m_notes;
	m_value = source.m_value;

	return *this;
}

void NoteSet::add_note(const Note& note) {
	m_notes.push_back(note);
}

void NoteSet::set_value(Value value) {
	m_value = value;
}

std::vector<Note> const& NoteSet::get_notes() const {
	return m_notes;
}

Value const& NoteSet::get_value() const {
	return m_value;
}

bool NoteSet::is_rest() const {
	return m_notes.size() == 0;
}