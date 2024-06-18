#pragma once

#include <SFML/Graphics.hpp>
#include "DataTypes.hpp"

constexpr const int MUSICAL_SYMBOL_DEFAULT_CHARACTER_SIZE = 100;

class MusicalSymbol : public sf::Drawable, public sf::Transformable {
public:
	MusicalSymbol();
	MusicalSymbol(const MusicalGlyph& glyph, const sf::Font& font, unsigned int character_size = MUSICAL_SYMBOL_DEFAULT_CHARACTER_SIZE);
	MusicalSymbol(const MusicalSymbol& source);
	~MusicalSymbol();

	MusicalSymbol& operator=(const MusicalSymbol& source);

	void set_glyph(const MusicalGlyph& glyph);
	const MusicalGlyph& get_glyph() const;

	void set_font(const sf::Font& font);
	const sf::Font& get_font() const;

	void set_character_size(unsigned int size);
	unsigned int get_character_size() const;

	void set_fill_color(const sf::Color& color);
	const sf::Color& get_fill_color() const;

	void set_outline_color(const sf::Color& color);
	const sf::Color& get_outline_color() const;

	void set_outline_thickness(float thickness);
	float get_outline_thickness() const;

	sf::FloatRect get_local_bounds() const;
	sf::FloatRect get_global_bounds() const;
	sf::Vector2f get_size() const;
	sf::Vector2f get_local_center() const;

	void use_font_baseline(bool use);
	bool is_using_font_baseline() const;

private:
	MusicalGlyph m_glyph{ MusicalGlyph::Null };
	const sf::Font* m_font{ nullptr };
	unsigned int m_character_size{ MUSICAL_SYMBOL_DEFAULT_CHARACTER_SIZE };

	sf::Color m_fill_color{ sf::Color::Black };
	sf::Color m_outline_color{ sf::Color::Transparent };
	float m_outline_thickness{ 0.0f };
	bool m_use_font_baseline{ false };

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};