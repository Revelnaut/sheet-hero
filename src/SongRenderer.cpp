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

		draw_measure(measure.treble_measure, draw_position, 10, target, states);
		draw_measure(measure.bass_measure, draw_position + sf::Vector2f(0.0f, m_settings.get_staff_height() + m_settings.get_staff_spacing()), -2, target, states);

		if (draw_position.x + (m_settings.get_measure_width(true) + m_settings.get_measure_width(false)) <= get_max_width()) {
			if (&measure != &m_song.get_measures().back()) { // If not last measure
				draw_position.x += m_settings.get_measure_width(false) + m_settings.get_bar_width() * 0.2f;
				bar.setPosition(draw_position);
				target.draw(bar, states);
				draw_position.x += m_settings.get_bar_width() * 0.8f;
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
	MusicalSymbol brace{ symbol_factory(MusicalGlyph::Brace) };
	brace.set_character_size(static_cast<unsigned int>(m_settings.get_grand_staff_height()));
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

void SongRenderer::draw_measure(const Measure& measure, sf::Vector2f position, int middle_c_offset, sf::RenderTarget& target, sf::RenderStates states) const {
	float measure_width{ m_settings.get_measure_width() };
	sf::Vector2f draw_position{ position };

	for (const NoteGroup& note_group : measure.get_note_groups()) {
		Value value{ note_group.get_value() };

		MusicalSymbol note_head{ symbol_factory(DataUtility::value_to_notehead_glyph(value)) };
		note_head.setOrigin(note_head.get_size().x / 2, note_head.get_size().y / 2);

		for (const Note& note : note_group.get_notes()) {
			float note_y{ (middle_c_offset - note.get_staff_position()) * m_settings.get_pitch_spacing() };
			note_head.setPosition(draw_position + sf::Vector2f{ 0.0f, note_y });
			target.draw(note_head, states);
		}

		switch (value) {
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

void SongRenderer::draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const
{
	float pitch_spacing{ m_settings.get_pitch_spacing() };
	float accidental_spacing{ m_settings.get_key_signature_accidental_spacing() };
	int accidental_count{ DataUtility::accidentals_in_key(key) };

	if (DataUtility::is_key_sharp(key)) {
		MusicalSymbol sharp_accidental{ symbol_factory(MusicalGlyph::AccidentalSharp) };
		sharp_accidental.use_font_baseline(true);
		if (accidental_count > 0) {
			sharp_accidental.setPosition(position + sf::Vector2f(0.0f, -pitch_spacing * 8));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 1) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing, -pitch_spacing * 5));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 2) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 9));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 3) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 4) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 3));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 5) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 7));
			target.draw(sharp_accidental, states);
		}
		if (accidental_count > 6) {
			sharp_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 4));
			target.draw(sharp_accidental, states);
		}
	}
	else if (DataUtility::is_key_flat(key)) {
		MusicalSymbol flat_accidental{ symbol_factory(MusicalGlyph::AccidentalFlat) };
		flat_accidental.use_font_baseline(true);
		if (accidental_count > 0) {
			flat_accidental.setPosition(position + sf::Vector2f(0.0f, -pitch_spacing * 4));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 1) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing, -pitch_spacing * 7));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 2) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 2, -pitch_spacing * 3));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 3) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 3, -pitch_spacing * 6));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 4) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 4, -pitch_spacing * 2));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 5) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 5, -pitch_spacing * 5));
			target.draw(flat_accidental, states);
		}
		if (accidental_count > 6) {
			flat_accidental.setPosition(position + sf::Vector2f(accidental_spacing * 6, -pitch_spacing * 1));
			target.draw(flat_accidental, states);
		}
	}
}
