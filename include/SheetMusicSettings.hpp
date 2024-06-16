#pragma once
#include <SFML/Graphics.hpp>
#include "DataTypes.hpp"

struct SheetMusicSettings {
	sf::Color color{ sf::Color::Black };
	sf::Color incorrect_color{ sf::Color::Red };
	sf::Color correct_color{ sf::Color::Green };

	float size{ 50.0f };
	float line_thickness_scale{ 1.0f };

	float clef_spacing_scale{ 1.0f };
	float key_signature_spacing_scale{ 1.0f };
	float key_signature_accidental_spacing_scale{ 1.0f };

	float bar_width_scale{ 1.0f };
	float measure_width_scale{ 1.0f };
	float first_measure_offset_scale{ 1.0f };

	float staff_spacing_scale{ 1.0f };
	float grand_staff_spacing_scale{ 1.0f };
	float grand_staff_brace_spacing_scale{ 0.0f };

	unsigned int get_font_size() const;
	float get_line_thickness() const;
	float get_line_spacing() const;
	float get_pitch_spacing() const;
	float get_clef_spacing() const;
	float get_key_signature_spacing() const;
	float get_key_signature_accidental_spacing() const;
	float get_bar_width() const;
	float get_measure_width(bool include_bars = false) const;
	float get_first_measure_offset() const;
	float get_first_measure_position(const Key& key) const;
	float get_staff_height() const;
	float get_staff_spacing() const;
	float get_grand_staff_height() const;
	float get_grand_staff_spacing() const;
	float get_grand_staff_brace_spacing() const;
};