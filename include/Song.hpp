#pragma once

#include "GrandStaff.hpp"
#include "MusicalData.hpp"
#include "Scale.hpp"
#include "TimeSignature.hpp"
#include "Note.hpp"
#include "NoteGroup.hpp"
#include <vector>
#include <unordered_map>

struct SongEvent {
	enum class Event {
		Null,
		NoteBegin,
		NoteSustain,
		NoteEnd,
	};

	Event event{};
	NoteGroup* note_group{};
};

class Song {
public:
	Song();
	Song(const Song& source);
	~Song();

	Song& operator=(const Song& source);

	void set_tempo(int tempo);
	int get_tempo() const;

	void set_key(Key key);
	const Key& get_key() const;
	const Scale& get_scale() const;

	void set_time_signature(const TimeSignature& time_signature);
	TimeSignature const& get_time_signature() const;

	void set_note_state(const Note& note, const NoteState& note_state);
	NoteState get_note_state(const Note& note) const;

	void set_grand_staff(const GrandStaff& grand_staff);
	const GrandStaff& get_grand_staff() const;

	SongEvent get_song_event_at(int index, int resolution);
private:
	int m_tempo{ 120 };
	TimeSignature m_time_signature{};
	Key m_key{ Key::CMajor };
	Scale m_scale{ Key::CMajor };

	std::unordered_map<unsigned int, NoteState> m_note_states{};
	GrandStaff m_grand_staff{};
};