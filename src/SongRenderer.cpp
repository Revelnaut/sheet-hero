#include "SongRenderer.hpp"
#include "MusicalSymbol.hpp"
#include <math.h>

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

	sf::RectangleShape bar{ sf::Vector2f{ 0.0f, m_settings.get_grand_staff_height() } };
	bar.setOutlineColor(m_settings.color);
	bar.setOutlineThickness(m_settings.get_line_thickness());

	bool new_line{ true };

	for (auto& measure : m_song.get_measures()) {
		if (new_line) {
			new_line = false;
			draw_grand_staff(draw_position, m_max_width, target, states);
			draw_position.x = m_settings.get_first_measure_position(m_song.get_key());
		}

		draw_measure(measure, draw_position, true, target, states);
		draw_measure(measure, draw_position, false, target, states);

		if (draw_position.x + (m_settings.get_measure_width(true) + m_settings.get_measure_width(false)) <= get_max_width()) {
			if (&measure != &m_song.get_measures().back()) { // If not last measure
				draw_position.x += m_settings.get_measure_width(false) + m_settings.get_bar_width() / 2;
				bar.setPosition(draw_position);
				target.draw(bar, states);
				draw_position.x += m_settings.get_bar_width() / 2;
			}
		}
		else {
			draw_position.x = 0.0f;
			draw_position.y += m_settings.get_grand_staff_height() + m_settings.get_grand_staff_spacing();
			new_line = true;
		}
	}
}

void SongRenderer::draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Vector2f draw_position{ position };

	// Brace
	MusicalSymbol brace = symbol_factory(MusicalGlyph::Brace);
	brace.set_character_size(m_settings.get_grand_staff_height());
	brace.setPosition(draw_position);
	target.draw(brace, states);

	draw_position.x += brace.get_size().x + m_settings.get_grand_staff_brace_spacing();

	// Horizontal line
	sf::RectangleShape staff_line{ sf::Vector2f(width - draw_position.x, 0.0f) };
	staff_line.setOutlineColor(m_settings.color);
	staff_line.setOutlineThickness(m_settings.get_line_thickness());

	// Treble staff
	staff_line.setPosition(draw_position);
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		if (i < 4) {
			staff_line.move(sf::Vector2f{ 0.0f, m_settings.get_line_spacing() });
		}
	}

	staff_line.move(sf::Vector2f{ 0.0f, m_settings.get_staff_spacing() });

	// Bass staff
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		staff_line.move(sf::Vector2f{ 0.0f, m_settings.get_line_spacing() });
	}

	// Vertical bars
	sf::RectangleShape vertical_line{ sf::Vector2f{ 0.0f, m_settings.get_grand_staff_height() } };
	vertical_line.setOutlineColor(m_settings.color);
	vertical_line.setOutlineThickness(m_settings.get_line_thickness());

	vertical_line.setPosition(draw_position + sf::Vector2f{ m_settings.get_line_thickness(), 0.0f });
	target.draw(vertical_line, states);

	vertical_line.setPosition(draw_position + sf::Vector2f{ staff_line.getSize().x - m_settings.get_line_thickness(), 0.0f });
	target.draw(vertical_line, states);

	// G clef
	draw_position.x += m_settings.get_clef_spacing();

	MusicalSymbol g_clef = symbol_factory(MusicalGlyph::ClefG);
	g_clef.use_font_baseline(true);
	g_clef.setOrigin(0, m_settings.get_line_spacing());
	g_clef.setPosition(draw_position);
	target.draw(g_clef, states);

	// F clef
	MusicalSymbol f_clef = symbol_factory(MusicalGlyph::ClefF);
	f_clef.use_font_baseline(true);
	f_clef.setOrigin(0, m_settings.get_line_spacing() * 3);
	f_clef.setPosition(draw_position + sf::Vector2f{ 0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing() });
	target.draw(f_clef, states);

	draw_position.x += g_clef.get_size().x + m_settings.get_key_signature_spacing();
	draw_key_signature(m_song.get_key(), draw_position, target, states);

	draw_position.y += m_settings.get_staff_height() + m_settings.get_staff_spacing() + m_settings.get_line_spacing();
	draw_key_signature(m_song.get_key(), draw_position, target, states);
}

void SongRenderer::draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, float size, const sf::Color& color) const {
	sf::Text text{};
	text.setString(symbol);
	text.setFont(m_music_font);
	if (size == 0) {
		text.setCharacterSize(m_settings.get_font_size());
	}
	else {
		text.setCharacterSize(static_cast<unsigned int>(size));
	}
	if (color == sf::Color::Transparent) {
		text.setFillColor(m_settings.color);
	}
	else {
		text.setFillColor(color);
	}
	text.setPosition(position);
	target.draw(text, states);
}

void SongRenderer::draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, sf::Vector2f scale, const sf::Color& color) const {
	sf::Text text{ symbol, m_music_font, m_settings.get_font_size() };
	text.setScale(scale);
	if (color == sf::Color::Transparent) {
		text.setFillColor(m_settings.color);
	}
	else {
		text.setFillColor(color);
	}
	text.setPosition(position);
	target.draw(text, states);
}

void SongRenderer::draw_measure(const Measure& measure, sf::Vector2f position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const {
	constexpr wchar_t NOTEHEAD_WHOLE = 0xE0A2;
	constexpr wchar_t NOTEHEAD_HALF = 0xE0A3;
	constexpr wchar_t NOTEHEAD_BLACK = 0xE0A4;
	constexpr wchar_t NOTE_FLAG_EIGHT = 0xE240;

	float ledger_line_width = m_settings.size / 2.0f;

	sf::RectangleShape ledger_line{ sf::Vector2f{ledger_line_width, 0.0f} };
	ledger_line.setOutlineThickness(m_settings.get_line_thickness());
	ledger_line.setOutlineColor(m_settings.color);
	ledger_line.setOrigin(ledger_line_width / 3.0f, 0.0f);

	sf::RectangleShape stem{ sf::Vector2f{0.0f, m_settings.get_pitch_spacing() * 7} };
	stem.setOutlineThickness(m_settings.get_line_thickness());
	stem.setOutlineColor(m_settings.color);

	sf::Vector2f draw_position{ position };

	const std::vector<NoteGroup>* note_groups{ nullptr };
	if (treble) {
		note_groups = &(measure.get_treble_note_groups());
	}
	else {
		note_groups = &(measure.get_bass_note_groups());
	}

	for (auto& note_group : *note_groups) {
		if (note_group.is_rest() == false) {
			wchar_t note_head{};

			switch (note_group.get_value()) {
			case Value::Whole:
				note_head = NOTEHEAD_WHOLE; break;
			case Value::Half:
				note_head = NOTEHEAD_HALF; break;
			default:
				note_head = NOTEHEAD_BLACK; break;
			}

			float second_interval_offset{ m_settings.size / 3.9f };
			float stem_offset{ m_settings.size / 3.7f };
			bool offset{ false };
			bool previous_offset{ false };
			int previous_staff_position{ note_group.get_notes()[0].get_staff_position() };
			bool stem_up = note_group.get_staff_mid_point() < 6;

			// Draw stem
			if (note_group.get_value() != Value::Whole) {
				float c4_position{};
				float middle_position{};
				if (treble) {
					c4_position = position.y + m_settings.get_pitch_spacing() * 10;
					middle_position = c4_position - m_settings.get_pitch_spacing() * 6;
				}
				else {
					c4_position = position.y + m_settings.get_staff_height() + m_settings.get_staff_spacing() - m_settings.get_pitch_spacing() * 2;
					middle_position = c4_position + m_settings.get_pitch_spacing() * 6;
				}

				float stem_start_y{}, stem_end_y{};

				stem_start_y = c4_position - (note_group.get_staff_max() * m_settings.get_pitch_spacing());
				if (stem_up) {
					stem_start_y -= m_settings.get_pitch_spacing() * 7;
				}
				stem_end_y = c4_position - (note_group.get_staff_min() * m_settings.get_pitch_spacing());
				if (!stem_up) {
					stem_end_y += m_settings.get_pitch_spacing() * 7;
				}

				if (stem_up) {
					if (stem_start_y > middle_position) {
						stem_start_y = middle_position;
					}
				}
				else {
					if (stem_end_y < middle_position) {
						stem_end_y = middle_position;
					}
				}

				sf::Vector2f stem_position{ draw_position };
				stem_position.x += stem_offset;
				stem_position.y = stem_start_y;
				stem.setSize(sf::Vector2f(0.0f, stem_end_y - stem_start_y));
				stem.setOutlineColor(m_settings.color);
				stem.setPosition(stem_position);
				target.draw(stem, states);

				if (note_group.get_value() == Value::Eight) {
					if (stem_up) {
						draw_symbol(NOTE_FLAG_EIGHT, sf::Vector2(draw_position.x + stem_offset - 0.5f, stem_start_y - m_settings.get_staff_height()), target, states, sf::Vector2f{ 0.95f, 0.95f });
					}
					else {
						draw_symbol(NOTE_FLAG_EIGHT, sf::Vector2(draw_position.x + stem_offset - 0.5f, stem_end_y + m_settings.get_staff_height()), target, states, sf::Vector2f{ 0.95f, -0.95f });
					}
				}
			}

			// Draw notes
			for (auto& note : note_group.get_notes()) {
				sf::Color note_color{ m_settings.color };

				draw_position.y = position.y - m_settings.get_pitch_spacing() * note.get_staff_position();

				if (treble) {
					draw_position.y += m_settings.get_pitch_spacing() * 2;
				}
				else {
					draw_position.y += m_settings.get_staff_height() + m_settings.get_staff_spacing() - m_settings.get_pitch_spacing() * 10;
				}

				if (previous_staff_position == note.get_staff_position() - 1 && previous_offset == false) {
					offset = true;
				}
				else {
					offset = false;
				}

				previous_staff_position = note.get_staff_position();
				previous_offset = offset;

				if ((offset && stem_up) || (!offset && !stem_up && note_group.get_value() != Value::Whole) || (offset && !stem_up && note_group.get_value() == Value::Whole)) {
					draw_symbol(note_head, draw_position + sf::Vector2f(second_interval_offset, 0), target, states, sf::Vector2f(1, 1), note_color);
				}
				else {
					draw_symbol(note_head, draw_position, target, states, sf::Vector2f(1, 1), note_color);
				}
			}

			// Draw ledger lines
			int top_ledger{ 12 };
			int bottom_ledger{ 0 };
			if (!treble) {
				top_ledger = 0;
				bottom_ledger = -12;
			}
			if (note_group.get_staff_max() >= top_ledger) {
				for (int i = top_ledger; i <= note_group.get_staff_max(); i += 2) {
					if (treble) {
						ledger_line.setPosition(draw_position.x + stem_offset / 2, position.y - m_settings.get_pitch_spacing() * (i - top_ledger + 2));
					}
					else {
						ledger_line.setPosition(draw_position.x + stem_offset / 2, position.y + m_settings.get_staff_height() + m_settings.get_staff_spacing() - m_settings.get_pitch_spacing() * (i - top_ledger + 2));
					}
					target.draw(ledger_line, states);
				}
			}
			if (note_group.get_staff_min() <= bottom_ledger) {
				for (int i = bottom_ledger; i >= note_group.get_staff_min(); i -= 2) {
					if (treble) {
						ledger_line.setPosition(draw_position.x + stem_offset / 2, position.y + m_settings.get_staff_height() + m_settings.get_pitch_spacing() * (2 - (i - bottom_ledger)));
					}
					else {
						ledger_line.setPosition(draw_position.x + stem_offset / 2, position.y + m_settings.get_staff_height() + m_settings.get_staff_spacing() - m_settings.get_pitch_spacing() * (i - top_ledger + 2));
					}
					target.draw(ledger_line, states);
				}
			}

			switch (note_group.get_value()) {
			case Value::Whole:
				draw_position.x += m_settings.get_measure_width();
				break;
			case Value::Half:
				draw_position.x += m_settings.get_measure_width() / 2;
				break;
			case Value::Quarter:
				draw_position.x += m_settings.get_measure_width() / 4;
				break;
			case Value::Eight:
				draw_position.x += m_settings.get_measure_width() / 8;
				break;
			}
		}
		else {
			// TODO: Rests
		}
	}
}

void SongRenderer::draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const
{
	constexpr wchar_t FLAT_ACCIDENTAL = 0xE260;
	constexpr wchar_t SHARP_ACCIDENTAL = 0xE262;
	float pitch_spacing{ m_settings.get_pitch_spacing() };
	float accidental_spacing{ m_settings.get_key_signature_accidental_spacing() };

	int accidental_count{ DataUtility::accidentals_in_key(key) };

	if (DataUtility::is_key_sharp(key)) {
		if (accidental_count > 0) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(0.0f, -pitch_spacing * 8), target, states);
		}
		if (accidental_count > 1) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing, -pitch_spacing * 5), target, states);
		}
		if (accidental_count > 2) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 9), target, states);
		}
		if (accidental_count > 3) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6), target, states);
		}
		if (accidental_count > 4) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 3), target, states);
		}
		if (accidental_count > 5) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 7), target, states);
		}
		if (accidental_count > 6) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 4), target, states);
		}
	}
	else if (DataUtility::is_key_flat(key)) {
		if (accidental_count > 0) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(0.0f, -pitch_spacing * 4), target, states);
		}
		if (accidental_count > 1) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing, -pitch_spacing * 7), target, states);
		}
		if (accidental_count > 2) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 3), target, states);
		}
		if (accidental_count > 3) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6), target, states);
		}
		if (accidental_count > 4) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 2), target, states);
		}
		if (accidental_count > 5) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 5), target, states);
		}
		if (accidental_count > 6) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 1), target, states);
		}
	}
}
