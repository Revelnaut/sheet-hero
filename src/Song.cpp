#include "Song.hpp"

Song::Song() {}

Song::Song(const Song& source)
	: m_tempo{ source.m_tempo },
	m_key{ source.m_key },
	m_scale{ source.m_scale },
	m_time_signature{ source.m_time_signature },
	m_grand_staff{ source.m_grand_staff }
{}

Song::~Song() {}

Song& Song::operator=(const Song& source) {
	m_tempo = source.m_tempo;
	m_key = source.m_key;
	m_scale = source.m_scale;
	m_time_signature = source.m_time_signature;
	m_grand_staff = source.m_grand_staff;

	return *this;
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

void Song::set_note_state(const Note& note, const NoteState& note_state) {
	m_note_states[note.get_id()] = note_state;
}

NoteState Song::get_note_state(const Note& note) const {
	if ( m_note_states.contains(note.get_id()) ) {
		return m_note_states.at(note.get_id());
	}
	return NoteState::Null;
}

void Song::set_grand_staff(const GrandStaff& grand_staff) {
	m_grand_staff = grand_staff;
}

const GrandStaff& Song::get_grand_staff() const {
	return m_grand_staff;
}
