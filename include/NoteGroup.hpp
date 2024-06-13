#pragma once

#include "Note.hpp"
#include <vector>

class NoteGroup {
public:
	NoteGroup();
	NoteGroup(Value value);
	NoteGroup(const NoteGroup& source);
	~NoteGroup();

	NoteGroup& operator=(const NoteGroup& source);

	void add_note(const Note& note);
	void set_value(Value value);

	const std::vector<Note>& get_notes() const;
	const Value& get_value() const;
	float get_staff_mid_point() const;
	float get_staff_max() const;
	float get_staff_min() const;

	bool is_rest() const;

private:
	std::vector<Note> m_notes{};
	Value m_value{ Value::Whole };
};