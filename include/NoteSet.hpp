#pragma once

#include "Note.hpp"
#include <vector>

class NoteSet {
public:
	NoteSet();
	NoteSet(Value value);
	NoteSet(const NoteSet& source);
	~NoteSet();

	NoteSet& operator=(const NoteSet& source);

	void add_note(const Note& note);
	void set_value(Value value);

	const std::vector<Note>& get_notes() const;
	const Value& get_value() const;
	
	bool is_rest() const;

private:
	std::vector<Note> m_notes{};
	Value m_value{ Value::Whole };
};