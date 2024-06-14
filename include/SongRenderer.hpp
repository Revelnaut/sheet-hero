#pragma once

#include <SFML/Graphics.hpp>
#include "Song.hpp"
#include <unordered_map>

class SongRenderer : public sf::Drawable, public sf::Transformable {
public:
	SongRenderer();
	SongRenderer(const Song& song);
	~SongRenderer();

	void set_song(const Song& song);
	const Song& get_song() const;

	void set_music_size(float size);
	void set_music_color(const sf::Color& color);

	const sf::Color& get_music_color() const;
	float get_music_size() const;

	void set_max_width(float max_width);
	float get_max_width() const;

private:
	Song m_song{};
	float m_position_in_song{};
	int m_note_group_at_position{};
	std::unordered_map<int, NoteState> m_note_states{};

	sf::Color m_music_color{ sf::Color::Black };
	sf::Color m_correct_color{ sf::Color::Green };
	sf::Color m_incorrect_color{ sf::Color::Red };
	float m_music_size{ 50.0f };
	sf::Font m_music_font{};
	float m_max_width{ 0.0 };

	void initialize();

	float get_note_gap() const;
	float get_vertical_pitch_separation() const;
	float get_line_thickness() const;
	float get_line_separation() const;
	float get_staff_separation() const;
	float get_staff_height() const;
	float get_grand_staff_separation() const;
	float get_grand_staff_height() const;
	float get_measure_width() const;
	float get_bar_margin() const;
	float get_beat_mark_margin() const;
	float get_beat_mark_size() const;
	float get_beat_division_mark_size() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, sf::Vector2f scale = sf::Vector2f(1, 1), const sf::Color& color = sf::Color::Transparent) const;
	void draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, float size, const sf::Color& color = sf::Color::Transparent) const;
	void draw_measure(const Measure& measure, sf::Vector2f position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const;
};
