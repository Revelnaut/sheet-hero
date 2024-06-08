#include "Measure.hpp"

Measure::Measure() {}

Measure::Measure(const Measure& source)
	: m_treble_sets{ source.m_treble_sets },
	m_bass_sets{ source.m_bass_sets },
	m_time_signature{ source.m_time_signature } {}

Measure::~Measure() {}

Measure& Measure::operator=(const Measure& source) {
	m_treble_sets = source.m_treble_sets;
	m_bass_sets = source.m_bass_sets;
	m_time_signature = source.m_time_signature;

	return *this;
}

void Measure::set_time_signature(const TimeSignature& time_signature) {
	m_time_signature = time_signature;
}

TimeSignature const& Measure::get_time_signature() const {
	return m_time_signature;
}

void Measure::add_treble_note_set(const NoteSet& note_set) {
	m_treble_sets.push_back(note_set);
}

std::vector<NoteSet> const& Measure::get_treble_note_sets() const {
	return m_treble_sets;
}

void Measure::add_bass_note_set(const NoteSet& note_set) {
	m_bass_sets.push_back(note_set);
}

std::vector<NoteSet> const& Measure::get_bass_note_sets() const {
	return m_bass_sets;
}

size_t Measure::get_note_set_count() const
{
	return m_treble_sets.size() > m_bass_sets.size() ? m_treble_sets.size() : m_bass_sets.size();
}
