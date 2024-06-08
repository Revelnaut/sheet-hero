#pragma once

#include "DataTypes.hpp"
#include "Scale.hpp"
#include "Measure.hpp"
#include <vector>

class Song {
public:
	Song();
	Song(const Song& source);
	~Song();

	Song& operator=(const Song& source);

	void add_measure(const Measure& measure);
	const std::vector<Measure>& get_measures() const;

	void set_tempo(int tempo);
	int get_tempo() const;

	void set_key(Key key);
	const Key& get_key() const;
	const Scale& get_scale() const;
private:
	int m_tempo{ 120 };
	Key m_key{ Key::CMajor };
	Scale m_scale{ Key::CMajor };

	std::vector<Measure> m_measures{};
};