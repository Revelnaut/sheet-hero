#include "NoteGroup.hpp"
#include <algorithm>
#include <cmath>

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

float NoteGroup::get_staff_mid_point() const
{
	int staff_position_sum{};
	for (const Note & note : m_notes) {
		staff_position_sum += note.get_staff_position();
	}
	return static_cast<float>(staff_position_sum) / static_cast<float>(m_notes.size());
}

float NoteGroup::get_staff_max() const
{
	return m_notes.back().get_staff_position();
}

float NoteGroup::get_staff_min() const
{
	return m_notes.front().get_staff_position();
}

bool NoteGroup::is_rest() const {
	return m_notes.size() == 0;
}