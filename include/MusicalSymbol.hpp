#pragma once

#include <SFML/Graphics.hpp>

enum class MusicalGlyph {
	Null = 0,

	Brace = 0xE000,
	ClefG = 0xE050,
	ClefF = 0xE062,

	TimeSignature0 = 0xE080,
	TimeSignature1 = 0xE081,
	TimeSignature2 = 0xE082,
	TimeSignature3 = 0xE083,
	TimeSignature4 = 0xE084,
	TimeSignature5 = 0xE085,
	TimeSignature6 = 0xE086,
	TimeSignature7 = 0xE087,
	TimeSignature8 = 0xE088,
	TimeSignature9 = 0xE089,

	AccidentalFlat = 0xE260,
	AccidentalNatural = 0xE261,
	AccidentalSharp = 0xE262,

	NoteheadWhole = 0xE0A2,
	NoteheadHalf = 0xE0A3,
	NoteheadBlack = 0xE0A4,

	Flag8thUp = 0xE240,
	Flag8thDown = 0xE241,
};

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

	sf::FloatRect get_bounds() const;

private:
	MusicalGlyph m_glyph{ MusicalGlyph::Null };
	const sf::Font* m_font{ nullptr };
	unsigned int m_character_size{ MUSICAL_SYMBOL_DEFAULT_CHARACTER_SIZE };

	sf::Color m_fill_color{ sf::Color::Black };
	sf::Color m_outline_color{ sf::Color::Transparent };
	float m_outline_thickness{ 0.0f };

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};