#include <cmath>
#include "SheetMusicSettings.hpp"

// These sizes are relative to the "size" member variable
constexpr static float LINE_THICKNESS{ 0.02f };
constexpr static float LINE_SPACING{ 0.25f };
constexpr static float PITCH_SPACING{ 0.125f };
constexpr static float CLEF_SPACING{ 0.25f };
constexpr static float TIME_SIGNATURE_SPACING{ 0.25f };
constexpr static float KEY_SIGNATURE_SPACING{ 0.25f };
constexpr static float KEY_SIGNATURE_ACCIDENTAL_SPACING{ 0.25f };
constexpr static float MEASURE_WIDTH{ 6.0f };
constexpr static float BAR_WIDTH{ 1.0f };
constexpr static float BAR_WIDTH_MIN{ 0.5f };
constexpr static float STAFF_SPACING{ 2.0f };
constexpr static float GRAND_STAFF_SPACING{ 3.0f };
constexpr static float GRAND_STAFF_BRACE_SPACING{ 0.5f };
constexpr static float FIRST_MEASURE_OFFSET{ 1.5f };

unsigned int SheetMusicSettings::get_font_size() const {
	return static_cast<unsigned int>(size);
}

float SheetMusicSettings::get_line_thickness() const
{
	float thickness = round(size * LINE_THICKNESS * line_thickness_scale);
	if (thickness < 1.0f) {
		return 1.0f;
	}
	else {
		return thickness;
	}
}

float SheetMusicSettings::get_line_spacing() const {
	return size * LINE_SPACING;
}

float SheetMusicSettings::get_pitch_spacing() const {
	return size * PITCH_SPACING;
}

float SheetMusicSettings::get_clef_spacing() const
{
	return size * CLEF_SPACING * clef_spacing_scale;
}

float SheetMusicSettings::get_time_signature_spacing() const
{
	return size * TIME_SIGNATURE_SPACING * time_signature_spacing_scale;
}

float SheetMusicSettings::get_key_signature_spacing() const
{
	return size * KEY_SIGNATURE_SPACING * key_signature_spacing_scale;
}

float SheetMusicSettings::get_key_signature_accidental_spacing() const
{
	return size * KEY_SIGNATURE_ACCIDENTAL_SPACING * key_signature_accidental_spacing_scale;
}

float SheetMusicSettings::get_bar_width() const {
	return size * BAR_WIDTH * bar_width_scale + (size * BAR_WIDTH_MIN);
}

float SheetMusicSettings::get_measure_width(bool include_bars) const {
	float width = size * MEASURE_WIDTH * measure_width_scale;
	if (include_bars) {
		width += get_bar_width();
	}
	return width;
}

float SheetMusicSettings::get_first_measure_offset() const {
	return size * FIRST_MEASURE_OFFSET * first_measure_offset_scale;
}

float SheetMusicSettings::get_first_measure_position(const Key& key) const {
	float accidentals_width{ get_key_signature_accidental_spacing() * DataUtility::accidentals_in_key(key) };
	return get_grand_staff_brace_spacing() + get_clef_spacing() + get_time_signature_spacing() + get_key_signature_spacing() + accidentals_width + get_first_measure_offset() + get_bar_width();
}

float SheetMusicSettings::get_staff_height() const {
	return size;
}

float SheetMusicSettings::get_staff_spacing() const
{
	return (size * STAFF_SPACING * staff_spacing_scale) + (get_line_spacing() * 2);
}

float SheetMusicSettings::get_grand_staff_height() const
{
	return get_staff_height() * 2 + get_staff_spacing();
}

float SheetMusicSettings::get_grand_staff_spacing() const
{
	return (size * GRAND_STAFF_SPACING * grand_staff_spacing_scale) + (get_line_spacing() * 2);
}

float SheetMusicSettings::get_grand_staff_brace_spacing() const
{
	return size * GRAND_STAFF_BRACE_SPACING * grand_staff_brace_spacing_scale;
}
