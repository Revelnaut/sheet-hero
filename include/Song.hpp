#pragma once

#include "MusicalData.hpp"
#include "Scale.hpp"
#include "GrandMeasure.hpp"
#include <vector>
#include <unordered_map>

class Song {
public:
	Song();
	Song(const Song& source);
	~Song();

	Song& operator=(const Song& source);

	void add_grand_measure(const GrandMeasure& measure);
	const std::vector<GrandMeasure>& get_grand_measures() const;

	int get_tick_at_position(float position) const;
	int get_beat_at_position(float position) const;
	const GrandMeasure& get_grand_measure_at_position(float position) const;
	const NoteGroup& get_note_group_at_position(float position, bool treble_staff) const;

	void set_tempo(int tempo);
	int get_tempo() const;

	void set_key(Key key);
	const Key& get_key() const;
	const Scale& get_scale() const;

	void set_time_signature(const TimeSignature& time_signature);
	TimeSignature const& get_time_signature() const;

	int get_beat_count() const;
	int get_tick_count() const;

	float get_normalized_tick_length() const;
	float get_normalized_beat_length() const;

	void set_note_state(const Note& note, const NoteState& note_state);
	NoteState get_note_state(const Note& note) const;
private:
	int m_tempo{ 120 };
	TimeSignature m_time_signature{};
	Key m_key{ Key::CMajor };
	Scale m_scale{ Key::CMajor };

	std::vector<GrandMeasure> m_grand_measures{};
	std::unordered_map<unsigned int, NoteState> m_note_states{};
};