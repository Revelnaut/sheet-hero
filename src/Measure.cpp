#include "Measure.hpp"

Measure::Measure() {}

Measure::Measure(const Measure& source)
	: m_treble_note_groups{ source.m_treble_note_groups },
	m_bass_note_groups{ source.m_bass_note_groups },
	m_time_signature{ source.m_time_signature } {}

Measure::~Measure() {}

Measure& Measure::operator=(const Measure& source) {
	m_treble_note_groups = source.m_treble_note_groups;
	m_bass_note_groups = source.m_bass_note_groups;
	m_time_signature = source.m_time_signature;

	return *this;
}

void Measure::set_time_signature(const TimeSignature& time_signature) {
	m_time_signature = time_signature;
}

TimeSignature const& Measure::get_time_signature() const {
	return m_time_signature;
}

void Measure::add_treble_note_group(const NoteGroup& note_group) {
	m_treble_note_groups.push_back(note_group);
}

std::vector<NoteGroup> const& Measure::get_treble_note_groups() const {
	return m_treble_note_groups;
}

void Measure::add_bass_note_group(const NoteGroup& note_group) {
	m_bass_note_groups.push_back(note_group);
}

std::vector<NoteGroup> const& Measure::get_bass_note_groups() const {
	return m_bass_note_groups;
}

size_t Measure::get_note_group_count() const
{
	return m_treble_note_groups.size() > m_bass_note_groups.size() ? m_treble_note_groups.size() : m_bass_note_groups.size();
}

int Measure::free_treble_space_in_eights() const
{
	int free_space = 8 * m_time_signature.get_top() / m_time_signature.get_bottom();
	for (auto note_group : m_treble_note_groups) {
		switch (note_group.get_value()) {
		case Value::Whole: free_space -= 8; break;
		case Value::Half: free_space -= 4; break;
		case Value::Quarter: free_space -= 2; break;
		case Value::Eight: free_space -= 1; break;
		}
	}
	return free_space;
}

int Measure::free_bass_space_in_eights() const
{
	int free_space = 8 * m_time_signature.get_top() / m_time_signature.get_bottom();
	for (auto note_group : m_bass_note_groups) {
		switch (note_group.get_value()) {
		case Value::Whole: free_space -= 8; break;
		case Value::Half: free_space -= 4; break;
		case Value::Quarter: free_space -= 2; break;
		case Value::Eight: free_space -= 1; break;
		}
	}
	return free_space;
}