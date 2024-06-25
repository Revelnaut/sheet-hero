#pragma once

#include "MusicalData.hpp"
#include "Song.hpp"
#include <utility>

class SongGenerator {
public:
	struct Configuration {
		std::pair<int, int> tempo_range{ 60, 120 };
	};

	struct MeasureConfiguration {
		std::pair<int, int> octave_range{};
		std::pair<Value, Value> value_range{ Value::Whole, Value::Eight };
		std::pair<int, int> note_group_size_range{ 1, 5 };

		int max_semitones_between_two_notes{ 3 };
	};

	Song generate(int measure_count) const;
private:
	MeasureConfiguration m_treble_configuration{ .octave_range{3, 5} };
	MeasureConfiguration m_bass_configuration{ .octave_range{1, 3} };

	Measure generate_measure(const MeasureConfiguration& configuration) const;
};