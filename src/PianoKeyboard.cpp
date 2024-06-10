#include "PianoKeyboard.hpp"

PianoKeyboard::PianoKeyboard() {}

PianoKeyboard::PianoKeyboard(const sf::Vector2f& size)
	: m_size{ size } {}

PianoKeyboard::PianoKeyboard(const PianoKeyboard& source)
	: m_size{ source.m_size } {}

PianoKeyboard::~PianoKeyboard() {}

PianoKeyboard& PianoKeyboard::operator=(const PianoKeyboard& source) {
	m_size = source.m_size;

	return *this;
}

void PianoKeyboard::set_size(const sf::Vector2f& size) {
	m_size = size;
}

const sf::Vector2f& PianoKeyboard::get_size() const {
	return m_size;
}

void PianoKeyboard::set_pressed_keys(const std::vector<int>& midi_pitches) {
	m_keys_pressed.clear();
	for (int i = 0; i < 88; ++i) {
		m_keys_pressed[i] = false;
	}

	for (int pitch : midi_pitches) {
		m_keys_pressed[pitch - 21] = true;
	}
}

void PianoKeyboard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	sf::RectangleShape white_key{ sf::Vector2f{m_size.x / 52.0f, m_size.y} };
	white_key.setOutlineColor(sf::Color::Black);
	white_key.setOutlineThickness(1.0f);

	sf::Vector2f draw_position{};

	for (int i = 0; i < 88; ++i) {
		int key_in_octave = (i + 9) % 12;
		switch (key_in_octave) {
		case 0:
		case 2:
		case 4:
		case 5:
		case 7:
		case 9:
		case 11:
			if (m_keys_pressed.at(i) == true) {
				white_key.setFillColor(sf::Color{ 0xFFFFFFFF });
			}
			else {
				white_key.setFillColor(sf::Color{ 0xEAEAEAFF });
			}
			white_key.setPosition(draw_position);
			target.draw(white_key, states);
			draw_position.x += white_key.getSize().x;
			break;
		}
	}

	sf::RectangleShape black_key{ sf::Vector2f{white_key.getSize().x * 0.6f, white_key.getSize().y * 0.7f} };
	black_key.setOutlineColor(sf::Color::Black);
	black_key.setOutlineThickness(1.0f);
	black_key.setOrigin(black_key.getSize().x / 2, 0);

	draw_position = { 0, 0 };

	for (int i = 0; i < 88; ++i) {
		int key_in_octave = (i + 9) % 12;
		switch (key_in_octave) {
		case 0:
		case 2:
		case 4:
		case 5:
		case 7:
		case 9:
		case 11:
			draw_position.x += white_key.getSize().x;
			break;
		case 1:
		case 3:
		case 6:
		case 8:
		case 10:
			if (m_keys_pressed.at(i) == true) {
				black_key.setFillColor(sf::Color{ 0x202020FF });
			}
			else {
				black_key.setFillColor(sf::Color{ 0x000000FF });
			}
			black_key.setPosition(draw_position);
			target.draw(black_key, states);
			break;
		}
	}
}
