#pragma once

#include "Staff.hpp"

class GrandStaff {
public:
	Staff treble{};
	Staff bass{};

	float get_whole_note_count() const;
};