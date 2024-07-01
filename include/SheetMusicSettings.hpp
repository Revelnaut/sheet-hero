#pragma once
#include <SFML/Graphics.hpp>
#include "MusicalData.hpp"

struct SheetMusicSettings {
	sf::Color color{ sf::Color::Black };
	sf::Color incorrect_color{ sf::Color::Red };
	sf::Color correct_color{ sf::Color::Green };
	sf::Color active_color{ sf::Color::Blue };

	sf::Color beat_on_color{ sf::Color::Blue };
	sf::Color beat_off_color{ {0xAAAAAAFF} };

	float size{ 40.0f };
	float line_thickness_scale{ 1.0f };

	float clef_spacing_scale{ 1.0f };
	float time_signature_spacing_scale{ 1.0f };
	float key_signature_spacing_scale{ 1.0f };
	float key_signature_accidental_spacing_scale{ 1.0f };

	float bar_width_scale{ 1.0f };
	float measure_width_scale{ 1.0f };
	float first_measure_offset_scale{ 1.0f };

	float staff_spacing_scale{ 1.0f };
	float grand_staff_spacing_scale{ 1.0f };
	float grand_staff_brace_spacing_scale{ 0.0f };

	float note_accidental_offset_scale{ 1.0f };

	float beat_size_scale{ 1.0f };
	float tick_size_scale{ 1.0f };

	unsigned int get_font_size() const;
	float get_line_thickness() const;
	float get_line_spacing() const;
	float get_pitch_spacing() const;
	float get_clef_spacing() const;
	float get_time_signature_spacing() const;
	float get_key_signature_spacing() const;
	float get_key_signature_accidental_spacing() const;
	float get_bar_width() const;
	float get_measure_width() const;
	float get_first_measure_offset() const;
	float get_first_measure_position(const Key& key) const;
	float get_staff_height() const;
	float get_staff_spacing() const;
	float get_grand_staff_height() const;
	float get_grand_staff_spacing() const;
	float get_grand_staff_brace_spacing() const;

	float get_note_accidental_offset() const;

	float get_beat_size() const;
	float get_tick_size() const;
};