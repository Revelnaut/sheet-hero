#pragma once

#include "DataTypes.hpp"
#include "Scale.hpp"
#include "Measure.hpp"
#include <vector>

class Song {
public:
	Song();
	~Song();

	void add_measure(const Measure& measure);
	const std::vector<Measure>& get_measures() const;
private:
	int tempo{ 120 };
	std::vector<Measure> m_measures{};
	Key m_key{};
	Scale m_scale{};
};