#include "Measure.hpp"

Measure::Measure() {}

Measure::Measure(const Measure& source)
	: m_note_groups{ source.m_note_groups } {}

Measure::~Measure() {}

Measure& Measure::operator=(const Measure& source) {
	m_note_groups = source.m_note_groups;

	return *this;
}

void Measure::add_note_group(const NoteGroup& note_group) {
	m_note_groups.push_back(note_group);
}

std::vector<NoteGroup> const& Measure::get_note_groups() const {
	return m_note_groups;
}

size_t Measure::get_note_group_count() const {
	return m_note_groups.size();
}

int Measure::free_space_in_eights(const TimeSignature& time_signature) const {
	int free_space = 8 * time_signature.get_numerator() / time_signature.get_denominator();
	for ( auto note_group : m_note_groups ) {
		switch ( note_group.get_value() ) {
		case Value::Whole: free_space -= 8; break;
		case Value::Half: free_space -= 4; break;
		case Value::Quarter: free_space -= 2; break;
		case Value::Eight: free_space -= 1; break;
		}
	}
	return free_space;
}
