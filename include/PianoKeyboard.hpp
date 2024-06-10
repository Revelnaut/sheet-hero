#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

class PianoKeyboard : public sf::Drawable, public sf::Transformable {
public:
	PianoKeyboard();
	PianoKeyboard(const sf::Vector2f& size);
	PianoKeyboard(const PianoKeyboard& source);
	~PianoKeyboard();

	PianoKeyboard& operator=(const PianoKeyboard& source);

	void set_size(const sf::Vector2f& size);
	const sf::Vector2f& get_size() const;

	void set_pressed_keys(const std::vector<int>& midi_pitches);
private:
	sf::Vector2f m_size{ 1000.0f, 100.0f };
	std::map<int, bool> m_keys_pressed{};

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};