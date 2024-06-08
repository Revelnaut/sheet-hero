#include "SongRenderer.hpp"
#include <math.h>

SongRenderer::SongRenderer() {
	m_music_font.loadFromFile("data/bravura/otf/Bravura.otf");
}

SongRenderer::~SongRenderer() {}

void SongRenderer::set_song(const Song& song) {
	m_song = song;
}

const Song& SongRenderer::get_song() const
{
	return m_song;
}

void SongRenderer::set_music_size(float size) {
	m_music_size = size;
}

void SongRenderer::set_music_color(const sf::Color& color) {
	m_music_color = color;
}

const sf::Color& SongRenderer::get_music_color() const {
	return m_music_color;
}

float SongRenderer::get_music_size() const {
	return m_music_size;
}

float SongRenderer::get_note_gap() const {
	return get_music_size() / 2.0f;
}

float SongRenderer::get_vertical_pitch_separation() const {
	return get_music_size() / 8.0f;
}

float SongRenderer::get_line_thickness() const {
	float thickness = round(get_music_size() / 50.0f);
	if (thickness < 1.0f) {
		thickness = 1.0f;
	}
	return thickness;
}

float SongRenderer::get_line_separation() const {
	return get_music_size() / 4.0f;
}

float SongRenderer::get_staff_separation() const {
	return get_music_size() * 2.0f;
}

float SongRenderer::get_staff_height() const {
	return get_music_size();
}

float SongRenderer::get_grand_staff_height() const {
	return get_staff_height() * 2.0f + get_staff_separation();
}

float SongRenderer::get_measure_width() const
{
	return get_music_size() * 4;
}

float SongRenderer::get_bar_margin() const
{
	return get_measure_width() / 8;
}

void SongRenderer::set_max_width(float max_width)
{
	m_max_width = max_width;
}

float SongRenderer::get_max_width() const
{
	return m_max_width;
}

void SongRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	draw_grand_staff(sf::Vector2f(0.0f, 0.0f), m_max_width, target, states);

	float left_margin = 100.0f;

	sf::Vector2f draw_position = sf::Vector2f(left_margin, 0.0f);

	sf::RectangleShape bar{ sf::Vector2f{ 0.0f, get_grand_staff_height() } };
	bar.setOutlineColor(get_music_color());
	bar.setOutlineThickness(get_line_thickness());

	for (auto measure : m_song.get_measures()) {
		draw_measure(measure, draw_position, true, target, states);

		if (draw_position.x + (get_measure_width() * 2) + (get_bar_margin() * 2) <= get_max_width()) {
			draw_position.x += get_measure_width() + get_bar_margin();
			bar.setPosition(draw_position);
			target.draw(bar, states);
			draw_position.x += get_bar_margin();
		}
		else {
			draw_position.x = left_margin;
			draw_position.y += get_grand_staff_height() + get_staff_separation();
			draw_grand_staff(sf::Vector2f(0.0f, draw_position.y), m_max_width, target, states);
		}
	}
}

void SongRenderer::draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape staff_line{ sf::Vector2f{width, 0.0f} };
	staff_line.setOutlineThickness(get_line_thickness());
	staff_line.setOutlineColor(m_music_color);

	// Treble staff
	staff_line.setPosition(position);
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		staff_line.move(sf::Vector2f{ 0.0f, get_line_separation()});
	}

	// Bass staff
	staff_line.move(sf::Vector2f{ 0.0f, get_staff_separation() - get_line_separation()});
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		staff_line.move(sf::Vector2f{ 0.0f, get_line_separation()});
	}

	// G clef
	draw_symbol(0xE050, position + sf::Vector2f{ get_line_separation(), -get_line_separation()}, get_music_color(), get_music_size(), target, states);

	// F clef
	draw_symbol(0xE062, position + sf::Vector2f{ get_line_separation(), get_staff_separation() + get_line_separation()}, get_music_color(), get_music_size(), target, states);

	// Brace
	draw_symbol(0xF402, position - sf::Vector2f{ get_line_separation(), 0.0f }, get_music_color(), get_grand_staff_height(), target, states);

	sf::RectangleShape vertical_line{ sf::Vector2f{ 0.0f, get_grand_staff_height() } };
	vertical_line.setOutlineColor(get_music_color());
	vertical_line.setOutlineThickness(get_line_thickness());

	vertical_line.setPosition(position + sf::Vector2f{ get_line_thickness(), 0.0f});
	target.draw(vertical_line, states);

	vertical_line.setPosition(position + sf::Vector2f{ width - get_line_thickness(), 0.0f});
	target.draw(vertical_line, states);
}

void SongRenderer::draw_symbol(wchar_t symbol, const sf::Vector2f& position, const sf::Color& color, float size, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Text text{ symbol, m_music_font, static_cast<unsigned int>(size) };
	text.setFillColor(m_music_color);
	text.setPosition(position);
	target.draw(text, states);
}

void SongRenderer::draw_measure(Measure& measure, sf::Vector2f position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const {
	constexpr wchar_t NOTE_STEM = 0xE210;
	constexpr wchar_t NOTEHEAD_WHOLE = 0xE0A2;
	constexpr wchar_t NOTEHEAD_HALF = 0xE0A3;
	constexpr wchar_t NOTEHEAD_BLACK = 0xE0A4;
	constexpr wchar_t NOTE_FLAG_EIGHT = 0xE240;

	sf::Vector2f draw_position{ position };
	if (treble) {
		draw_position.y += get_vertical_pitch_separation() * 2;
	}
	else {
		draw_position.y += get_vertical_pitch_separation() * 4;
	}

	for (auto note_set : measure.get_treble_note_sets()) {
		wchar_t note_head{};

		switch (note_set.get_value()) {
		case Value::Whole:
			note_head = NOTEHEAD_WHOLE; break;
		case Value::Half:
			note_head = NOTEHEAD_HALF; break;
		default:
			note_head = NOTEHEAD_BLACK; break;
		}

		for (auto note : note_set.get_notes()) {
			draw_position.y = position.y - get_vertical_pitch_separation() * note.get_staff_position();
			if (treble) {
				draw_position.y += get_vertical_pitch_separation() * 2;
			}
			else {
				draw_position.y += get_vertical_pitch_separation() * 4;
			}
			draw_symbol(note_head, draw_position, sf::Color::Green, get_music_size(), target, states);
		}

		switch (note_set.get_value()) {
		case Value::Whole:
			draw_position.x += get_measure_width();
			break;
		case Value::Half:
			draw_position.x += get_measure_width() / 2;
			break;
		case Value::Quarter:
			draw_position.x += get_measure_width() / 4;
			break;
		case Value::Eight:
			draw_position.x += get_measure_width() / 8;
			break;
		}
	}
}