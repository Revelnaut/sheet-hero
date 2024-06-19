#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Song.hpp"
#include "SheetMusicSettings.hpp"
#include "MusicalSymbol.hpp"

class SongRenderer : public sf::Drawable, public sf::Transformable {
public:
	SongRenderer();
	SongRenderer(const Song& song);
	~SongRenderer();

	void set_song(const Song& song);
	const Song& get_song() const;

	void set_max_width(float max_width);
	float get_max_width() const;

	void set_settings(const SheetMusicSettings& settings);
	SheetMusicSettings& get_settings();
private:
	sf::Font m_music_font{};
	SheetMusicSettings m_settings{};

	Song m_song{};
	float m_position_in_song{};

	float m_max_width{ 0.0 };

	void initialize();

	MusicalSymbol symbol_factory(const MusicalGlyph& glyph) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_measure(const Measure& measure, sf::Vector2f position, int middle_c_offset, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const;
};
