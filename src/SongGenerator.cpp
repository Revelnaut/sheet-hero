#include "SongGenerator.hpp"
#include "effolkronium/random.hpp"

using Random = effolkronium::random_static;

Song SongGenerator::generate(int measure_count) const {
	Song song{};
	for ( int m = 0; m < measure_count; ++m ) {
		GrandMeasure grand_measure;
		grand_measure.treble_measure = generate_measure(m_treble_configuration);
		grand_measure.bass_measure = generate_measure(m_bass_configuration);

		song.add_grand_measure(grand_measure);
	}
	return song;
}

Measure SongGenerator::generate_measure(const MeasureConfiguration& configuration) const {
	return Measure();
}
