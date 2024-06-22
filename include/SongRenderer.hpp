#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Song.hpp"
#include "SheetMusicSettings.hpp"
#include "MusicalSymbol.hpp"

class SongRenderer {
public:
	SongRenderer();
	~SongRenderer();

	void set_bounds(const sf::FloatRect& bounds);
	const sf::FloatRect& get_bounds() const;

	void set_settings(const SheetMusicSettings& settings);
	SheetMusicSettings& get_settings();

	void set_playing_position(double position);
	double get_playing_position() const;

	void render(const Song& song, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
private:
	sf::Font m_music_font{};
	SheetMusicSettings m_settings{};

	double m_playing_position{};

	sf::FloatRect m_bounds{};

	void initialize();

	MusicalSymbol symbol_factory(const MusicalGlyph& glyph) const;

	void draw_grand_staff(const Song& song, sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_measure(const Song& song, const Measure& measure, sf::Vector2f position, int middle_c_offset, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_key_signature(const Song& song, const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const;
};
