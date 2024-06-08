#pragma once

#include <SFML/Graphics.hpp>
#include "Measure.hpp"
#include "Scale.hpp"
#include <unordered_map>

class Game : public sf::Drawable, public sf::Transformable {
public:
	Game();
	~Game();

	void set_game_area(const sf::FloatRect& game_area);
	const sf::FloatRect& get_game_area() const;
	void process_event(const sf::Window& window, const sf::Event& event);

	const sf::Color& get_music_color() const;
	float get_music_size() const;

	float get_note_gap() const;
	float get_vertical_pitch_separation() const;
	float get_line_thickness() const;
	float get_line_separation() const;
	float get_staff_separation() const;
	float get_staff_height() const;
	float get_grand_staff_height() const;

private:
	sf::Color m_music_color{ sf::Color::Black };
	float m_music_size{ 50.0f };
	sf::Font m_music_font{};

	sf::FloatRect m_game_area{};

	Key current_key{ Key::CMajor };
	Scale current_scale{ Key::CMajor };

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_symbol(wchar_t symbol, const sf::Vector2f& position, const sf::Color& color, float size, sf::RenderTarget& target, sf::RenderStates states) const;
	void draw_measure(Measure& measure, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const;
};