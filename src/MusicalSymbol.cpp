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
	m_outline_thickness{ source.m_outline_thickness },
	m_use_font_baseline{ source.m_use_font_baseline }
{}

MusicalSymbol::~MusicalSymbol() {}

MusicalSymbol& MusicalSymbol::operator=(const MusicalSymbol& source) {
	m_glyph = source.m_glyph;
	m_font = source.m_font;
	m_character_size = source.m_character_size;
	m_fill_color = source.m_fill_color;
	m_outline_color = source.m_outline_color;
	m_outline_thickness = source.m_outline_thickness;
	m_use_font_baseline = source.m_use_font_baseline;

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

sf::FloatRect MusicalSymbol::get_local_bounds() const
{
	sf::FloatRect bounds{};
	if (m_font) {
		auto glyph_data = m_font->getGlyph(static_cast<sf::Uint32>(m_glyph), m_character_size, false, m_outline_thickness);
		if (m_use_font_baseline) {
			bounds = glyph_data.bounds;
			bounds.top += m_character_size;
		}
		else {
			bounds = sf::FloatRect{ sf::Vector2f(0.0f, 0.0f), glyph_data.bounds.getSize() };
		}
	}
	return bounds;
}

sf::FloatRect MusicalSymbol::get_global_bounds() const
{
	return sf::FloatRect{ getPosition(), get_local_bounds().getSize() };
}

sf::Vector2f MusicalSymbol::get_size() const
{
	return get_local_bounds().getSize();
}

sf::Vector2f MusicalSymbol::get_local_center() const
{
	return sf::Vector2f{ get_size().x / 2.0f, get_size().y / 2.0f };
	return sf::Vector2f{ get_size().x / 2.0f, get_size().y / 2.0f };
}

void MusicalSymbol::use_font_baseline(bool use) {
	m_use_font_baseline = use;
}

bool MusicalSymbol::is_using_font_baseline() const {
	return m_use_font_baseline;
}

void MusicalSymbol::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (m_font) {
		states.transform *= getTransform();

		sf::Glyph glyph_data = m_font->getGlyph(static_cast<sf::Uint32>(m_glyph), m_character_size, false, m_outline_thickness);

		sf::Sprite symbol_renderer{};
		if (m_use_font_baseline) {
			symbol_renderer.setPosition(sf::Vector2f(0.0f, static_cast<float>(m_character_size)) + glyph_data.bounds.getPosition());
		}
		symbol_renderer.setTexture(m_font->getTexture(m_character_size), false);
		symbol_renderer.setTextureRect(glyph_data.textureRect);
		symbol_renderer.setColor(m_fill_color);
		target.draw(symbol_renderer, states);
	}
}
