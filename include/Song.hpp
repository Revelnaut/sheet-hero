#pragma once

#include "DataTypes.hpp"
#include "Scale.hpp"
#include "GrandMeasure.hpp"
#include <vector>

class Song {
public:
	Song();
	Song(const Song& source);
	~Song();

	Song& operator=(const Song& source);

	void add_measure(const GrandMeasure& measure);
	const std::vector<GrandMeasure>& get_measures() const;

	void set_tempo(int tempo);
	int get_tempo() const;

	void set_key(Key key);
	const Key& get_key() const;
	const Scale& get_scale() const;

	void set_time_signature(const TimeSignature& time_signature);
	TimeSignature const& get_time_signature() const;
private:
	int m_tempo{ 120 };
	TimeSignature m_time_signature{};
	Key m_key{ Key::CMajor };
	Scale m_scale{ Key::CMajor };

	std::vector<GrandMeasure> m_measures{};
};