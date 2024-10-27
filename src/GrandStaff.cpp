#include "GrandStaff.hpp"
#include <cmath>

float GrandStaff::get_whole_note_count() const {
	return std::max(treble.get_whole_note_count(), bass.get_whole_note_count());
}
