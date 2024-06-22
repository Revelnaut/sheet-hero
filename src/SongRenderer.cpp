#include "SongRenderer.hpp"
#include "MusicalSymbol.hpp"
#include "LineShape.hpp"

SongRenderer::SongRenderer() {
	initialize();
}

SongRenderer::SongRenderer(const Song& song) {
	initialize();
	set_song(song);
}

SongRenderer::~SongRenderer() {}

void SongRenderer::set_song(const Song& song) {
	m_song = song;
}

const Song& SongRenderer::get_song() const {
	return m_song;
}

void SongRenderer::initialize() {
	m_music_font.loadFromFile("data/bravura/otf/Bravura.otf");
}

void SongRenderer::set_max_width(float max_width) {
	m_max_width = max_width;
}

float SongRenderer::get_max_width() const {
	return m_max_width;
}

void SongRenderer::set_settings(const SheetMusicSettings& settings) {
	m_settings = settings;
}

SheetMusicSettings& SongRenderer::get_settings() {
	return m_settings;
}

MusicalSymbol SongRenderer::symbol_factory(const MusicalGlyph& glyph) const {
	return MusicalSymbol{ glyph, m_music_font, m_settings.get_font_size() };
}

void SongRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	sf::Vector2f draw_position{};

	LineShape bar{ 0.0f, 0.0f, 0.0f, m_settings.get_grand_staff_height() };
	bar.set_color(m_settings.color);
	bar.set_thickness(m_settings.get_line_thickness());

	sf::CircleShape beat{ m_settings.get_beat_size() / 2.0f };
	beat.setOrigin(beat.getRadius(), beat.getRadius());
	beat.setFillColor(m_settings.beat_off_color);

	sf::CircleShape tick{ m_settings.get_tick_size() / 2.0f };
	tick.setOrigin(tick.getRadius(), tick.getRadius());
	tick.setFillColor(m_settings.beat_off_color);

	bool new_line{ true };

	for ( auto& measure : m_song.get_measures() ) {
		if ( new_line ) {
			new_line = false;
			draw_grand_staff(draw_position, m_max_width, target, states);
			draw_position.x = m_settings.get_first_measure_position(m_song.get_key());
		}

		const TimeSignature& time_signature{ m_song.get_time_signature() };
		float beat_length{ m_settings.get_measure_width(false) / static_cast<float>( time_signature.get_numerator() ) };
		float beat_position_y{ m_settings.get_staff_height() + m_settings.get_staff_spacing() / 2.0f };
		for ( int b = 0; b < time_signature.get_numerator(); ++b ) {
			beat.setPosition(draw_position);
			beat.move(b * beat_length, beat_position_y);
			target.draw(beat, states);

			for ( int t = 1; t < 4; ++t ) {
				tick.setPosition(beat.getPosition());
				tick.move(t * beat_length / 4.0f, 0.0f);
				target.draw(tick, states);
			}
		}

		draw_measure(measure.treble_measure, draw_position, 10, target, states);
		draw_measure(measure.bass_measure, draw_position + sf::Vector2f(0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing()), -2, target, states);

		if ( draw_position.x + ( m_settings.get_measure_width(true) + m_settings.get_measure_width(false) ) <= get_max_width() ) {
			if ( &measure != &m_song.get_measures().back() ) { // If not last measure
				draw_position.x += m_settings.get_measure_width(false) + m_settings.get_bar_width() * 0.2f;
				bar.setPosition(draw_position);
				target.draw(bar, states);
				draw_position.x += m_settings.get_bar_width() * 0.8f;
			}
		} else {
			draw_position.x = 0.0f;
			draw_position.y += m_settings.get_grand_staff_height() + m_settings.get_grand_staff_spacing();
			new_line = true;
		}
	}
}

void SongRenderer::draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Vector2f draw_position{ position };

	// Brace
	MusicalSymbol brace{ symbol_factory(MusicalGlyph::Brace) };
	brace.set_character_size(static_cast<unsigned int>( m_settings.get_grand_staff_height() ));
	brace.setPosition(draw_position);
	target.draw(brace, states);

	draw_position.x += brace.get_size().x + m_settings.get_grand_staff_brace_spacing();

	// Horizontal line
	LineShape horizontal_line{ 0.0f, 0.0f, width - draw_position.x, 0.0f };
	horizontal_line.set_color(m_settings.color);
	horizontal_line.set_thickness(m_settings.get_line_thickness());

	// Treble staff
	horizontal_line.setPosition(draw_position);
	for ( int i = 0; i < 5; ++i ) {
		target.draw(horizontal_line, states);
		if ( i < 4 ) {
			horizontal_line.move(sf::Vector2f{ 0.0f, m_settings.get_line_spacing() });
		}
	}

	horizontal_line.move(sf::Vector2f{ 0.0f, m_settings.get_staff_spacing() });

	// Bass staff
	for ( int i = 0; i < 5; ++i ) {
		target.draw(horizontal_line, states);
		horizontal_line.move(sf::Vector2f{ 0.0f, m_settings.get_line_spacing() });
	}

	// Vertical bars
	LineShape vertical_line{ 0.0f, 0.0f, 0.0f, m_settings.get_grand_staff_height() };
	vertical_line.set_color(m_settings.color);
	vertical_line.set_thickness(m_settings.get_line_thickness());
	vertical_line.setPosition(draw_position);
	target.draw(vertical_line, states);

	vertical_line.setPosition(draw_position + sf::Vector2f{ width - draw_position.x, 0.0f });
	target.draw(vertical_line, states);

	// G clef
	draw_position.x += m_settings.get_clef_spacing();

	MusicalSymbol g_clef{ symbol_factory(MusicalGlyph::ClefG) };
	g_clef.use_font_baseline(true);
	g_clef.setOrigin(0, m_settings.get_line_spacing());
	g_clef.setPosition(draw_position);
	target.draw(g_clef, states);

	// F clef
	MusicalSymbol f_clef{ symbol_factory(MusicalGlyph::ClefF) };
	f_clef.use_font_baseline(true);
	f_clef.setOrigin(0, m_settings.get_line_spacing() * 3);
	f_clef.setPosition(draw_position + sf::Vector2f{ 0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing() });
	target.draw(f_clef, states);

	// Time signature
	const TimeSignature& time_signature{ m_song.get_time_signature() };

	MusicalSymbol ts_numerator{ symbol_factory(DataUtility::int_to_time_signature_glyph(time_signature.get_numerator())) };
	MusicalSymbol ts_denominator{ symbol_factory(DataUtility::int_to_time_signature_glyph(time_signature.get_denominator())) };

	draw_position.x += g_clef.get_size().x + m_settings.get_time_signature_spacing();

	ts_numerator.setPosition(draw_position);
	ts_denominator.setPosition(draw_position.x, draw_position.y + m_settings.get_line_spacing() * 2);
	target.draw(ts_numerator, states);
	target.draw(ts_denominator, states);

	ts_numerator.move(0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing());
	ts_denominator.move(0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing());
	target.draw(ts_numerator, states);
	target.draw(ts_denominator, states);

	// Key signature
	draw_position.x += ts_numerator.get_size().x + m_settings.get_key_signature_spacing();
	draw_key_signature(m_song.get_key(), draw_position, target, states);

	draw_position.y += m_settings.get_staff_height() + m_settings.get_staff_spacing() + m_settings.get_line_spacing();
	draw_key_signature(m_song.get_key(), draw_position, target, states);
}

#include <iostream>

void SongRenderer::draw_measure(const Measure& measure, sf::Vector2f position, int middle_c_offset, sf::RenderTarget& target, sf::RenderStates states) const {
	float measure_width{ m_settings.get_measure_width() / m_song.get_time_signature().get_ratio() };

	sf::Vector2f draw_position{ position };
	int staff_middle_line = middle_c_offset - 4;
	float staff_middle_position_y = position.y + m_settings.get_line_spacing() * 2;

	Scale measure_scale{ m_song.get_scale() };

	for ( const NoteGroup& note_group : measure.get_note_groups() ) {
		// Note head
		Value value{ note_group.get_value() };
		MusicalSymbol note_head{ symbol_factory(DataUtility::value_to_notehead_glyph(value)) };
		note_head.setOrigin(note_head.get_size().x / 2, note_head.get_size().y / 2);

		LineShape ledger_line{};
		ledger_line.set_thickness(m_settings.get_line_thickness());
		ledger_line.set_color(m_settings.color);

		float ledger_line_point_x = note_head.get_size().x * 0.8;

		ledger_line.set_point_1(-ledger_line_point_x, 0.0f);
		ledger_line.set_point_2(ledger_line_point_x, 0.0f);

		bool stem_direction_up = note_group.get_staff_mid_point() <= staff_middle_line;

		// Iterate through notes and draw them
		float close_note_offset = note_head.get_size().x * 0.9;
		bool note_offset{ true };
		int previous_note_position{};

		if ( !stem_direction_up ) {
			close_note_offset = -close_note_offset;
		}

		for ( const Note& note : note_group.get_notes() ) {
			float note_y{ ( middle_c_offset - note.get_staff_position() ) * m_settings.get_pitch_spacing() };
			note_head.setPosition(draw_position + sf::Vector2f{ 0.0f, note_y });

			if ( note.get_staff_position() == previous_note_position + 1 && note_offset == false ) {
				note_offset = true;
				note_head.move(close_note_offset, 0.0f);
				if ( stem_direction_up ) {
					ledger_line.set_point_2(ledger_line_point_x + close_note_offset, 0.0f);
				} else {
					ledger_line.set_point_1(-ledger_line_point_x + close_note_offset, 0.0f);
				}
			} else {
				note_offset = false;
			}

			target.draw(note_head, states);

			previous_note_position = note.get_staff_position();
		}

		// Draw ledger lines
		if ( note_group.get_staff_max() >= middle_c_offset + 2 ) {
			for ( int i = middle_c_offset + 2; i <= note_group.get_staff_max(); i += 2 ) {
				ledger_line.setPosition(draw_position.x, position.y - ( i - middle_c_offset ) * m_settings.get_pitch_spacing());
				target.draw(ledger_line, states);
			}
		}

		if ( note_group.get_staff_min() <= middle_c_offset - 10 ) {
			for ( int i = middle_c_offset - 10; i >= note_group.get_staff_min(); i -= 2 ) {
				ledger_line.setPosition(draw_position.x, position.y - ( i - middle_c_offset ) * m_settings.get_pitch_spacing());
				target.draw(ledger_line, states);
			}
		}

		// Draw stem
		if ( value != Value::Whole ) {
			LineShape stem{ };
			stem.set_thickness(m_settings.size / 25.0f);
			stem.set_color(m_settings.color);

			float bottom_note_y = draw_position.y + ( middle_c_offset - note_group.get_staff_min() ) * m_settings.get_pitch_spacing();
			float top_note_y = draw_position.y + ( middle_c_offset - note_group.get_staff_max() ) * m_settings.get_pitch_spacing();
			float stem_overflow = m_settings.get_pitch_spacing() * 7;
			float stem_x = draw_position.x;
			float stem_offset_to_note_x = note_head.get_size().x / 2 - stem.get_thickness() / 2;
			float stem_offset_to_note_y = note_head.get_size().y / 4;


			if ( stem_direction_up ) {
				stem_x += stem_offset_to_note_x;
			} else {
				stem_x -= stem_offset_to_note_x;
			}

			if ( stem_direction_up ) {
				stem.set_points(stem_x, bottom_note_y - stem_offset_to_note_y, stem_x, top_note_y - stem_overflow);
				if ( stem.get_point_2().y > staff_middle_position_y ) {
					stem.set_point_2(stem_x, staff_middle_position_y);
				}
			} else {
				stem.set_points(stem_x, top_note_y + stem_offset_to_note_y, stem_x, bottom_note_y + stem_overflow);
				if ( stem.get_point_2().y < staff_middle_position_y ) {
					stem.set_point_2(stem_x, staff_middle_position_y);
				}
			}

			target.draw(stem, states);

			if ( value == Value::Eight ) {
				MusicalSymbol flag{ symbol_factory(MusicalGlyph::Null) };
				if ( stem_direction_up ) {
					flag.set_glyph(MusicalGlyph::Flag8thUp);
				} else {
					flag.set_glyph(MusicalGlyph::Flag8thDown);
				}
				flag.use_font_baseline(true);
				flag.setPosition(stem.get_point_2() - sf::Vector2f(0.0f, m_settings.size));
				target.draw(flag, states);
			}
		}

		// Move the draw position forward according to the note's value/length
		switch ( value ) {
		case Value::Whole:
			draw_position.x += measure_width;
			break;
		case Value::Half:
			draw_position.x += measure_width / 2.0f;
			break;
		case Value::Quarter:
			draw_position.x += measure_width / 4.0f;
			break;
		case Value::Eight:
			draw_position.x += measure_width / 8.0f;
			break;
		}
	}
}

void SongRenderer::draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const {
	float pitch_spacing{ m_settings.get_pitch_spacing() };
	float accidental_spacing{ m_settings.get_key_signature_accidental_spacing() };
	int accidental_count{ DataUtility::accidental_count_in_key(key) };

	if ( DataUtility::is_key_sharp(key) ) {
		MusicalSymbol sharp_accidental{ symbol_factory(MusicalGlyph::AccidentalSharp) };
		sharp_accidental.use_font_baseline(true);
		if ( accidental_count > 0 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(0.0f, -pitch_spacing * 8));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 1 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing, -pitch_spacing * 5));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 2 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 9));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 3 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 4 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 3));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 5 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 7));
			target.draw(sharp_accidental, states);
		}
		if ( accidental_count > 6 ) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 4));
			target.draw(sharp_accidental, states);
		}
	} else if ( DataUtility::is_key_flat(key) ) {
		MusicalSymbol flat_accidental{ symbol_factory(MusicalGlyph::AccidentalFlat) };
		flat_accidental.use_font_baseline(true);
		if ( accidental_count > 0 ) {
			flat_accidental.setPosition(position + sf::Vector2f(0.0f, -pitch_spacing * 4));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 1 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing, -pitch_spacing * 7));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 2 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 3));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 3 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 4 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 2));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 5 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 5));
			target.draw(flat_accidental, states);
		}
		if ( accidental_count > 6 ) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 1));
			target.draw(flat_accidental, states);
		}
	}
}
