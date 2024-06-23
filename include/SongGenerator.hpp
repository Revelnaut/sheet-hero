#pragma once

#include "MusicalData.hpp"

class SongGenerator {
public:
	struct Configuration {
		int treble_octave_min{ 3 };
		int treble_octave_max{ 5 };
		int bass_octave_min{ 1 };
		int bass_octave_max{ 3 };

		bool generate_treble{ true };
		bool generate_bass{ true };

		Value treble_value_min{ Value::Whole };
		Value treble_value_max{ Value::Eight };
	};
private:
	Configuration m_configuration{};
};