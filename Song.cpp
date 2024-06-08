#include "Song.hpp"

Song::Song() {}
Song::~Song() {}

void Song::add_measure(const Measure& measure) {

}

const std::vector<Measure>& Song::get_measures() const {
	return m_measures;
}