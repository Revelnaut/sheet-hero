#include "Song.hpp"

Song::Song() {}

Song::Song(const Song& source)
	: m_tempo{ source.m_tempo },
	m_measures{ source.m_measures },
	m_key{ source.m_key },
	m_scale{source.m_scale}
{}

Song::~Song() {}

Song& Song::operator=(const Song& source)
{
	m_tempo = source.m_tempo;
	m_measures = source.m_measures;
	m_key = source.m_key;
	m_scale = source.m_scale;

	return *this;
}

void Song::add_measure(const GrandMeasure& measure) {
	m_measures.push_back(measure);
}

const std::vector<GrandMeasure>& Song::get_measures() const {
	return m_measures;
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