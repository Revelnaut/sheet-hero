#include "SongRenderer.hpp"
#include "MusicalSymbol.hpp"
#include "LineShape.hpp"

#include <algorithm>

SongRenderer::SongRenderer() {
	initialize();
}

SongRenderer::~SongRenderer() {}

void SongRenderer::set_bounds(const sf::FloatRect& bounds) {
	m_bounds = bounds;
}

const sf::FloatRect& SongRenderer::get_bounds() const {
	return m_bounds;
}

void SongRenderer::initialize() {
	m_music_font.loadFromFile("data/bravura/otf/Bravura.otf");
}

void SongRenderer::set_settings(const SheetMusicSettings& settings) {
	m_settings = settings;
}

SheetMusicSettings& SongRenderer::get_settings() {
	return m_settings;
}

static int get_playing_tick(const Song& song, float position) {
	return static_cast<int>( static_cast<float>( song.get_tick_count() ) * position);
}

void SongRenderer::render(const Song& song, float playing_position, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Vector2f draw_position{m_bounds.getPosition()};

	LineShape bar{ 0.0f, 0.0f, 0.0f, m_settings.get_grand_staff_height() };
	bar.set_color(m_settings.color);
	bar.set_thickness(m_settings.get_line_thickness());

	sf::CircleShape beat{ m_settings.get_beat_size() / 2.0f };
	beat.setOrigin(beat.getRadius(), beat.getRadius());
	beat.setFillColor(m_settings.beat_off_color);

	sf::CircleShape tick{ m_settings.get_tick_size() / 2.0f };
	tick.setOrigin(tick.getRadius(), tick.getRadius());
	tick.setFillColor(m_settings.beat_off_color);

	int measure_counter{ 0 };
	int tick_counter{ 0 };

	bool new_line{ true };

	for ( auto& measure : song.get_grand_measures() ) {
		if ( new_line ) {
			new_line = false;
			draw_grand_staff(song, draw_position, m_bounds.width, target, states);
			draw_position.x = m_bounds.left + m_settings.get_first_measure_position(song.get_key());
		}

		const TimeSignature& time_signature{ song.get_time_signature() };
		float beat_length{ m_settings.get_measure_width() / static_cast<float>( time_signature.get_numerator() ) };
		float beat_position_y{ m_settings.get_staff_height() + m_settings.get_staff_spacing() / 2.0f };
		for ( int b = 0; b < time_signature.get_numerator(); ++b ) {
			beat.setPosition(draw_position);
			beat.move(b * beat_length, beat_position_y);

			if ( get_playing_tick(song, playing_position) == tick_counter ) {
				beat.setFillColor(m_settings.active_color);
			} else {
				beat.setFillColor(m_settings.beat_off_color);
			}

			target.draw(beat, states);
			++tick_counter;

			for ( int t = 1; t < 4; ++t ) {
				tick.setPosition(beat.getPosition());
				tick.move(t * beat_length / 4.0f, 0.0f);

				if ( get_playing_tick(song, playing_position) == tick_counter ) {
					tick.setFillColor(m_settings.active_color);
				} else {
					tick.setFillColor(m_settings.beat_off_color);
				}

				target.draw(tick, states);
				++tick_counter;
			}
		}

		draw_measure(song, measure.treble_measure, measure_counter, draw_position, 10, target, states);
		draw_measure(song, measure.bass_measure, measure_counter, draw_position + sf::Vector2f(0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing()), -2, target, states);

		if ( draw_position.x + ( m_settings.get_measure_width() * 2 + m_settings.get_bar_width() ) <= m_bounds.width + m_bounds.left ) {
			if ( &measure != &song.get_grand_measures().back() ) { // If not last measure
				draw_position.x += m_settings.get_measure_width() + m_settings.get_bar_width() * 0.2f;
				bar.setPosition(draw_position);
				target.draw(bar, states);
				draw_position.x += m_settings.get_bar_width() * 0.8f;
			}
		} else {
			draw_position.x = m_bounds.left;
			draw_position.y += m_settings.get_grand_staff_height() + m_settings.get_grand_staff_spacing();
			new_line = true;
		}

		++measure_counter;
	}
}

MusicalSymbol SongRenderer::symbol_factory(const MusicalGlyph& glyph) const {
	return MusicalSymbol{ glyph, m_music_font, m_settings.get_font_size() };
}

void SongRenderer::draw_grand_staff(const Song& song, sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Vector2f draw_position{ position };

	// Brace
	MusicalSymbol brace{ symbol_factory(MusicalGlyph::Brace) };
	brace.set_character_size(static_cast<unsigned int>( m_settings.get_grand_staff_height() ));
	brace.setPosition(draw_position);
	target.draw(brace, states);

	draw_position.x += brace.get_size().x + m_settings.get_grand_staff_brace_spacing();

	// Horizontal line
	LineShape horizontal_line{ 0.0f, 0.0f, width, 0.0f };
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

	vertical_line.setPosition(draw_position + sf::Vector2f{ width, 0.0f });
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
	const TimeSignature& time_signature{ song.get_time_signature() };

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
	draw_key_signature(song, song.get_key(), draw_position, target, states);

	draw_position.y += m_settings.get_staff_height() + m_settings.get_staff_spacing() + m_settings.get_line_spacing();
	draw_key_signature(song, song.get_key(), draw_position, target, states);
}

void SongRenderer::draw_measure(const Song& song, const Measure& measure, int number, sf::Vector2f position, int middle_c_offset, sf::RenderTarget& target, sf::RenderStates states) const {
	int tick{ number * song.get_time_signature().get_numerator() * 4 };
	float measure_width{ m_settings.get_measure_width() / song.get_time_signature().get_ratio() };

	sf::Vector2f draw_position{ position };
	int staff_middle_line = middle_c_offset - 4;
	float staff_middle_position_y = position.y + m_settings.get_line_spacing() * 2;

	std::unordered_map<int, Accidental> staff_line_accidentals{};

	for ( const NoteGroup& note_group : measure.get_note_groups() ) {
		// Note head
		Value value{ note_group.get_value() };
		MusicalSymbol note_head{ symbol_factory(DataUtility::value_to_notehead_glyph(value)) };
		note_head.setOrigin(note_head.get_size().x / 2, note_head.get_size().y / 2);

		LineShape ledger_line{};
		ledger_line.set_thickness(m_settings.get_line_thickness());
		ledger_line.set_color(m_settings.color);

		float ledger_line_point_x = note_head.get_size().x * 0.8f;

		ledger_line.set_point_1(-ledger_line_point_x, 0.0f);
		ledger_line.set_point_2(ledger_line_point_x, 0.0f);

		bool stem_direction_up = note_group.get_staff_mid_point() <= staff_middle_line;

		// Iterate through notes and draw them
		float close_note_offset = note_head.get_size().x * 0.9f;
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

			const NoteState& note_state = song.get_note_state(note);
			
			switch ( note_state ) {
			case NoteState::Null:
			case NoteState::Normal:
				note_head.set_fill_color(m_settings.color);
				break;
			case NoteState::Correct:
				note_head.set_fill_color(m_settings.correct_color);
				break;
			case NoteState::Incorrect:
				note_head.set_fill_color(m_settings.incorrect_color);
				break;
			}

			target.draw(note_head, states);

			// Draw accidentals. Only necessary accidentals are drawn, even though every note is assigned an accidental.
			if ( staff_line_accidentals[note.get_staff_position()] == Accidental::Null ) {
				staff_line_accidentals[note.get_staff_position()] = song.get_scale().get_accidental(note.get_pitch_class());
			}

			bool different_accidental = staff_line_accidentals[note.get_staff_position()] != note.get_accidental();

			if ( different_accidental ) {
				MusicalSymbol accidental{ symbol_factory(MusicalGlyph::Null) };
				accidental.use_font_baseline(true);

				switch ( note.get_accidental() ) {
				case Accidental::Flat:
					accidental.set_glyph(MusicalGlyph::AccidentalFlatSmall);
					break;
				case Accidental::Natural:
					accidental.set_glyph(MusicalGlyph::AccidentalNaturalSmall);
					break;
				case Accidental::Sharp:
					accidental.set_glyph(MusicalGlyph::AccidentalSharpSmall);
					break;
				}
				accidental.setPosition(note_head.getPosition() - sf::Vector2f(m_settings.get_note_accidental_offset(), m_settings.get_pitch_spacing() * 8));

				target.draw(accidental, states);
			}

			staff_line_accidentals[note.get_staff_position()] = note.get_accidental();

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
			tick += 16;
			break;
		case Value::Half:
			draw_position.x += measure_width / 2.0f;
			tick += 8;
			break;
		case Value::Quarter:
			draw_position.x += measure_width / 4.0f;
			tick += 4;
			break;
		case Value::Eight:
			draw_position.x += measure_width / 8.0f;
			tick += 2;
			break;
		}
	}
}

void SongRenderer::draw_key_signature(const Song& song, const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const {
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
