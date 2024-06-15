#include "MusicalSymbol.hpp"

MusicalSymbol::MusicalSymbol() {}

MusicalSymbol::MusicalSymbol(const MusicalGlyph& glyph, const sf::Font& font, unsigned int character_size) :
	m_glyph{ glyph },
	m_font{ &font },
	m_character_size{ character_size }
{}

MusicalSymbol::MusicalSymbol(const MusicalSymbol& source) :
	m_glyph{ source.m_glyph },
	m_font{ source.m_font },
	m_character_size{ source.m_character_size },
	m_fill_color{ source.m_fill_color },
	m_outline_color{ source.m_outline_color },
	m_outline_thickness{ source.m_outline_thickness }
{}

MusicalSymbol::~MusicalSymbol() {}

MusicalSymbol& MusicalSymbol::operator=(const MusicalSymbol& source) {
	m_glyph = source.m_glyph;
	m_font = source.m_font;
	m_character_size = source.m_character_size;
	m_fill_color = source.m_fill_color;
	m_outline_color = source.m_outline_color;
	m_outline_thickness = source.m_outline_thickness;

	return *this;
}

void MusicalSymbol::set_glyph(const MusicalGlyph& glyph) {
	m_glyph = glyph;
}

const MusicalGlyph& MusicalSymbol::get_glyph() const {
	return m_glyph;
}

void MusicalSymbol::set_font(const sf::Font& font) {
	m_font = &font;
}

const sf::Font& MusicalSymbol::get_font() const {
	return *m_font;
}

void MusicalSymbol::set_character_size(unsigned int size) {
	m_character_size = size;
}

unsigned int MusicalSymbol::get_character_size() const {
	return m_character_size;
}

void MusicalSymbol::set_fill_color(const sf::Color& color) {
	m_fill_color = color;
}

const sf::Color& MusicalSymbol::get_fill_color() const {
	return m_fill_color;
}

void MusicalSymbol::set_outline_color(const sf::Color& color) {
	m_outline_color = color;
}

const sf::Color& MusicalSymbol::get_outline_color() const {
	return m_outline_color;
}

void MusicalSymbol::set_outline_thickness(float thickness) {
	m_outline_thickness = thickness;
}

float MusicalSymbol::get_outline_thickness() const {
	return m_outline_thickness;
}

sf::FloatRect MusicalSymbol::get_bounds() const
{
	auto glyph_data = m_font->getGlyph(static_cast<sf::Uint32>(m_glyph), m_character_size, false, m_outline_thickness);
	return glyph_data.bounds;
}

void MusicalSymbol::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	sf::Text text_renderer{ static_cast<wchar_t>(m_glyph), *m_font, m_character_size };
	text_renderer.setFillColor(m_fill_color);
	text_renderer.setOutlineColor(m_outline_color);
	text_renderer.setOutlineThickness(m_outline_thickness);

	target.draw(text_renderer, states);
}
