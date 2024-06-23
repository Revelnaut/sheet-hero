#include "Song.hpp"

Song::Song() {}

Song::Song(const Song& source)
	: m_tempo{ source.m_tempo },
	m_grand_measures{ source.m_grand_measures },
	m_key{ source.m_key },
	m_scale{ source.m_scale },
	m_time_signature{ source.m_time_signature } {}

Song::~Song() {}

Song& Song::operator=(const Song& source) {
	m_tempo = source.m_tempo;
	m_grand_measures = source.m_grand_measures;
	m_key = source.m_key;
	m_scale = source.m_scale;
	m_time_signature = source.m_time_signature;

	return *this;
}

void Song::add_grand_measure(const GrandMeasure& measure) {
	m_grand_measures.push_back(measure);
}

const std::vector<GrandMeasure>& Song::get_grand_measures() const {
	return m_grand_measures;
}

int Song::get_tick_at_position(float position) const {
	return static_cast<int>( static_cast<float>( get_tick_count() ) * position );
}

int Song::get_beat_at_position(float position) const {
	return get_tick_at_position(position) / 4;
}

const GrandMeasure& Song::get_grand_measure_at_position(float position) const {
	int index = get_beat_at_position(position) / get_time_signature().get_numerator();
	return m_grand_measures[index];
}

const NoteGroup& Song::get_note_group_at_position(float position, bool treble_staff) const {
	auto& grand_measure = get_grand_measure_at_position(position);
	int tick = get_tick_at_position(position) % ( get_time_signature().get_numerator() * 4 );

	int i = 0;
	int note_group_index = 0;
	while ( i < tick ) {
		Value value{};
		if ( treble_staff ) {
			value = grand_measure.treble_measure.get_note_groups().at(note_group_index).get_value();
		} else {
			value = grand_measure.bass_measure.get_note_groups().at(note_group_index).get_value();
		}
		i += DataUtility::value_to_tick(value);
		if ( i <= tick ) {
			++note_group_index;
		}
	}
	if ( treble_staff ) {
		return grand_measure.treble_measure.get_note_groups().at(note_group_index);
	}
	return grand_measure.bass_measure.get_note_groups().at(note_group_index);
}

void Song::set_tempo(int tempo) {
	m_tempo = tempo;
}

int Song::get_tempo() const {
	return m_tempo;
}

void Song::set_key(Key key) {
	m_key = key;
	m_scale.set_key(key);
}

const Key& Song::get_key() const {
	return m_key;
}

const Scale& Song::get_scale() const {
	return m_scale;
}

void Song::set_time_signature(const TimeSignature& time_signature) {
	m_time_signature = time_signature;
}

TimeSignature const& Song::get_time_signature() const {
	return m_time_signature;
}

int Song::get_beat_count() const {
	return m_grand_measures.size() * m_time_signature.get_numerator();
}

int Song::get_tick_count() const {
	return get_beat_count() * 4;
}

float Song::get_normalized_tick_length() const {
	return 1.0f / static_cast<float>( get_tick_count() );
}

float Song::get_normalized_beat_length() const {
	return 1.0f / static_cast<float>( get_beat_count() );
}

void Song::set_note_state(const Note& note, const NoteState& note_state) {
	m_note_states[note.get_id()] = note_state;
}

const NoteState& Song::get_note_state(const Note& note) const {
	if ( m_note_states.contains(note.get_id()) ) {
		return m_note_states.at(note.get_id());
	}
	return NoteState::Null;
}
