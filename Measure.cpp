#include "Measure.hpp"

Measure::Measure() {}

Measure::Measure(const Measure& source)
	: m_sets{ source.m_sets }, m_time_signature{ source.m_time_signature } {}

Measure::~Measure() {}

Measure& Measure::operator=(const Measure& source) {
	m_sets = source.m_sets;
	m_time_signature = source.m_time_signature;

	return *this;
}

void Measure::add_note_set(const NoteSet& note_set) {
	m_sets.push_back(note_set);
}

void Measure::set_time_signature(const TimeSignature& time_signature) {
	m_time_signature = time_signature;
}

TimeSignature const& Measure::get_time_signature() const {
	return m_time_signature;
}

NoteSet const& Measure::get_note_set(int index) const {
	return m_sets.at(index);
}

size_t Measure::get_note_set_count() const {
	return m_sets.size();
}

std::vector<NoteSet> const& Measure::get_note_sets() const {
	return m_sets;
}

