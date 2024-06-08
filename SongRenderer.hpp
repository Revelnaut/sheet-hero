#pragma once

#include <SFML/Graphics.hpp>
#include "Song.hpp"

class SongRenderer : public sf::Drawable, public sf::Transformable {
public:
	SongRenderer();
	~SongRenderer();

	void set_song(const Song& song);
	const Song& get_song() const;

	void set_music_size(float size);
	void set_music_color(const sf::Color& color);

	const sf::Color& get_music_color() const;
	float get_music_size() const;

	float get_note_gap() const;
	float get_vertical_pitch_separation() const;
	float get_line_thickness() const;
	float get_line_separation() const;
	float get_staff_separation() const;
	float get_staff_height() const;
	float get_grand_staff_height() const;
	float get_measure_width() const;
	float get_bar_margin() const;

	void set_max_width(float max_width);
	float get_max_width() const;

private:
	Song m_song{};

	sf::Color m_music_color{ sf::Color::Black };
	float m_music_size{ 50.0f };
	sf::Font m_music_font{};

	float m_max_width{ 0.0 };

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_symbol(wchar_t symbol, const sf::Vector2f& position, const sf::Color& color, float size, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_measure(Measure& measure, sf::Vector2f position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_note_set(NoteSet& note_set, sf::Vector2f midline_position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const;
};