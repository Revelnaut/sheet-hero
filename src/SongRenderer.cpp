#include "SongRenderer.hpp"
#include <math.h>

SongRenderer::SongRenderer() {
	initialize();
}

SongRenderer::SongRenderer(const Song& song) {
	initialize();
	set_song(song);
}

SongRenderer::~SongRenderer() {}

void SongRenderer::set_song(const Song& song) {
	m_song = song;

	// Set initial note states
	m_note_states.clear();
	for (auto& measure : m_song.get_measures()) {
		for (auto& note_group : measure.get_treble_note_groups()) {
			for (auto& note : note_group.get_notes()) {
				m_note_states[note.get_id()] = NoteState::Normal;
			}
		}

		for (auto& note_group : measure.get_bass_note_groups()) {
			for (auto& note : note_group.get_notes()) {
				m_note_states[note.get_id()] = NoteState::Normal;
			}
		}
	}
}

const Song& SongRenderer::get_song() const
{
	return m_song;
}

void SongRenderer::set_music_size(float size) {
	m_music_size = size;
}

void SongRenderer::set_music_color(const sf::Color& color) {
	m_music_color = color;
}

const sf::Color& SongRenderer::get_music_color() const {
	return m_music_color;
}

float SongRenderer::get_music_size() const {
	return m_music_size;
}

void SongRenderer::initialize() {
	m_music_font.loadFromFile("data/bravura/otf/Bravura.otf");
}

float SongRenderer::get_note_gap() const {
	return get_music_size() / 2.0f;
}

float SongRenderer::get_vertical_pitch_separation() const {
	return get_music_size() / 8.0f;
}

float SongRenderer::get_line_thickness() const {
	float thickness = round(get_music_size() / 50.0f);
	if (thickness < 1.0f) {
		thickness = 1.0f;
	}
	return thickness;
}

float SongRenderer::get_line_separation() const {
	return get_music_size() / 4.0f;
}

float SongRenderer::get_staff_separation() const {
	return get_music_size() * 2.0f;
}

float SongRenderer::get_staff_height() const {
	return get_music_size();
}

float SongRenderer::get_grand_staff_separation() const
{
	return get_music_size() * 3.0f;
}

float SongRenderer::get_grand_staff_height() const {
	return get_staff_height() * 2.0f + get_staff_separation();
}

float SongRenderer::get_measure_width() const
{
	return get_music_size() * 6;
}

float SongRenderer::get_bar_margin() const
{
	return get_measure_width() / 16;
}

float SongRenderer::get_beat_mark_margin() const
{
	return get_music_size();
}

float SongRenderer::get_beat_mark_size() const
{
	return get_music_size() / 4;
}

float SongRenderer::get_beat_division_mark_size() const
{
	return get_beat_mark_size() / 2;
}

void SongRenderer::set_max_width(float max_width)
{
	m_max_width = max_width;
}

float SongRenderer::get_max_width() const
{
	return m_max_width;
}

void SongRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	

	float left_margin = 180.0f;

	sf::Vector2f draw_position = sf::Vector2f(left_margin, 0.0f);

	sf::RectangleShape bar{ sf::Vector2f{ 0.0f, get_grand_staff_height() } };
	bar.setOutlineColor(get_music_color());
	bar.setOutlineThickness(get_line_thickness());

	sf::RectangleShape beat{ sf::Vector2f{ get_beat_mark_size() / 2, get_beat_mark_size() } };
	beat.setOrigin(get_beat_mark_size() / 4, get_beat_mark_size() / 2);
	beat.setFillColor(sf::Color{ 0, 0, 0, 40 });

	bool new_line{ true };

	for (auto& measure : m_song.get_measures()) {
		if (new_line) {
			new_line = false;
			draw_grand_staff(sf::Vector2f(0.0f, draw_position.y), m_max_width, target, states);
		}
		sf::Vector2f beat_position{ draw_position };
		beat_position.x += get_music_size() / 4;
		beat_position.y -= m_music_size;
		for (int i = 0; i < 4; ++i) {
			beat.setScale(1, 1);
			beat.setPosition(beat_position);
			target.draw(beat, states);

			for (int j = 1; j < 4; ++j) {
				beat.setScale(0.5, 0.5);
				beat.setPosition(beat_position + sf::Vector2f(get_measure_width() / 16 * j, 0));
				target.draw(beat, states);
			}

			beat_position.x += get_measure_width() / 4;
		}

		draw_measure(measure, draw_position, true, target, states);
		draw_measure(measure, draw_position, false, target, states);

		if (draw_position.x + (get_measure_width() * 2) + (get_bar_margin() * 2) <= get_max_width()) {
			if (&measure != &m_song.get_measures().back()) { // If not last measure
				draw_position.x += get_measure_width() + get_bar_margin();
				bar.setPosition(draw_position);
				target.draw(bar, states);
				draw_position.x += get_bar_margin();
			}
		}
		else {
			draw_position.x = left_margin;
			draw_position.y += get_grand_staff_height() + get_grand_staff_separation();
			new_line = true;
		}
	}
}

void SongRenderer::draw_grand_staff(sf::Vector2f position, float width, sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape staff_line{ sf::Vector2f{width, 0.0f} };
	staff_line.setOutlineThickness(get_line_thickness());
	staff_line.setOutlineColor(m_music_color);

	// Treble staff
	staff_line.setPosition(position);
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		staff_line.move(sf::Vector2f{ 0.0f, get_line_separation() });
	}

	// Bass staff
	staff_line.move(sf::Vector2f{ 0.0f, get_staff_separation() - get_line_separation() });
	for (int i = 0; i < 5; ++i) {
		target.draw(staff_line, states);
		staff_line.move(sf::Vector2f{ 0.0f, get_line_separation() });
	}

	draw_key_signature(m_song.get_key(), position + sf::Vector2f(50.0f, 0.0f), target, states);
	draw_key_signature(m_song.get_key(), position + sf::Vector2f(50.0f, get_staff_separation() + get_staff_height() + get_vertical_pitch_separation() * 2), target, states);

	// G clef
	draw_symbol(0xE050, position + sf::Vector2f{ get_line_separation(), -get_line_separation() }, target, states);

	// F clef
	draw_symbol(0xE062, position + sf::Vector2f{ get_line_separation(), get_staff_separation() + get_line_separation() }, target, states);

	// Brace
	draw_symbol(0xF402, position - sf::Vector2f{ get_line_separation(), 0.0f }, target, states, get_grand_staff_height());

	sf::RectangleShape vertical_line{ sf::Vector2f{ 0.0f, get_grand_staff_height() } };
	vertical_line.setOutlineColor(get_music_color());
	vertical_line.setOutlineThickness(get_line_thickness());

	vertical_line.setPosition(position + sf::Vector2f{ get_line_thickness(), 0.0f });
	target.draw(vertical_line, states);

	vertical_line.setPosition(position + sf::Vector2f{ width - get_line_thickness(), 0.0f });
	target.draw(vertical_line, states);
}

void SongRenderer::draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, float size, const sf::Color& color) const {
	sf::Text text{};
	text.setString(symbol);
	text.setFont(m_music_font);
	if (size == 0) {
		text.setCharacterSize(m_music_size);
	}
	else {
		text.setCharacterSize(size);
	}
	if (color == sf::Color::Transparent) {
		text.setFillColor(m_music_color);
	}
	else {
		text.setFillColor(color);
	}
	text.setPosition(position);
	target.draw(text, states);
}

void SongRenderer::draw_symbol(wchar_t symbol, const sf::Vector2f& position, sf::RenderTarget& target, sf::RenderStates states, sf::Vector2f scale, const sf::Color& color) const {
	sf::Text text{ symbol, m_music_font, static_cast<unsigned int>(m_music_size) };
	text.setScale(scale);
	if (color == sf::Color::Transparent) {
		text.setFillColor(m_music_color);
	}
	else {
		text.setFillColor(color);
	}
	text.setPosition(position);
	target.draw(text, states);
}

void SongRenderer::draw_measure(const Measure& measure, sf::Vector2f position, bool treble, sf::RenderTarget& target, sf::RenderStates states) const {
	constexpr wchar_t NOTEHEAD_WHOLE = 0xE0A2;
	constexpr wchar_t NOTEHEAD_HALF = 0xE0A3;
	constexpr wchar_t NOTEHEAD_BLACK = 0xE0A4;
	constexpr wchar_t NOTE_FLAG_EIGHT = 0xE240;

	sf::RectangleShape stem{ sf::Vector2f{0.0f, get_vertical_pitch_separation() * 7}};
	stem.setOutlineThickness(get_line_thickness());
	stem.setOutlineColor(get_music_color());

	sf::Vector2f draw_position{ position };

	const std::vector<NoteGroup>* note_groups{ nullptr };
	if (treble) {
		note_groups = &(measure.get_treble_note_groups());
	}
	else {
		note_groups = &(measure.get_bass_note_groups());
	}

	for (auto& note_group : *note_groups) {
		if (note_group.is_rest() == false) {
			wchar_t note_head{};

			switch (note_group.get_value()) {
			case Value::Whole:
				note_head = NOTEHEAD_WHOLE; break;
			case Value::Half:
				note_head = NOTEHEAD_HALF; break;
			default:
				note_head = NOTEHEAD_BLACK; break;
			}

			float second_interval_offset{ get_music_size() / 3.9f };
			float stem_offset{ get_music_size() / 3.7f };
			bool offset{ false };
			bool previous_offset{ false };
			int previous_staff_position{ note_group.get_notes()[0].get_staff_position() };
			bool stem_up = note_group.get_staff_mid_point() < 6;

			// Draw stem
			if (note_group.get_value() != Value::Whole) {
				float c4_position{};
				float middle_position{};
				if (treble) {
					c4_position = position.y + get_vertical_pitch_separation() * 10;
					middle_position = c4_position - get_vertical_pitch_separation() * 6;
				}
				else {
					c4_position = position.y + get_staff_height() + get_staff_separation() - get_vertical_pitch_separation() * 2;
					middle_position = c4_position + get_vertical_pitch_separation() * 6;
				}
				
				float stem_start_y{}, stem_end_y{};

				stem_start_y = c4_position - (note_group.get_staff_max() * get_vertical_pitch_separation());
				if (stem_up) {
					stem_start_y -= get_vertical_pitch_separation() * 7;
				}
				stem_end_y = c4_position - (note_group.get_staff_min() * get_vertical_pitch_separation());
				if (!stem_up) {
					stem_end_y += get_vertical_pitch_separation() * 7;
				}

				if (stem_up) {
					if (stem_start_y > middle_position) {
						stem_start_y = middle_position;
					}
				}
				else {
					if (stem_end_y < middle_position) {
						stem_end_y = middle_position;
					}
				}

				sf::Vector2f stem_position{ draw_position };
				stem_position.x += stem_offset;
				stem_position.y = stem_start_y;
				stem.setSize(sf::Vector2f(0.0f, stem_end_y - stem_start_y));
				stem.setOutlineColor(m_music_color);
				stem.setPosition(stem_position);
				target.draw(stem, states);

				if (note_group.get_value() == Value::Eight) {
					if (stem_up) {
						draw_symbol(NOTE_FLAG_EIGHT, sf::Vector2(draw_position.x + stem_offset - 0.5f, stem_start_y - get_staff_height()), target, states, sf::Vector2f(0.95, 0.95));
					}
					else {
						draw_symbol(NOTE_FLAG_EIGHT, sf::Vector2(draw_position.x + stem_offset - 0.5f, stem_end_y + get_staff_height()), target, states, sf::Vector2f(0.95, -0.95));
					}
				}
			}

			// Draw notes
			for (auto& note : note_group.get_notes()) {
				sf::Color note_color{ m_music_color };

				switch (m_note_states.at(note.get_id())) {
				case NoteState::Correct: note_color = m_correct_color; break;
				case NoteState::Incorrect: note_color = m_incorrect_color;  break;
				}

				draw_position.y = position.y - get_vertical_pitch_separation() * note.get_staff_position();

				if (treble) {
					draw_position.y += get_vertical_pitch_separation() * 2;
				}
				else {
					draw_position.y += get_staff_height() + get_staff_separation() - get_vertical_pitch_separation() * 10;
				}

				if (previous_staff_position == note.get_staff_position() - 1 && previous_offset == false) {
					offset = true;
				}
				else {
					offset = false;
				}

				previous_staff_position = note.get_staff_position();
				previous_offset = offset;

				if ((offset && stem_up) || (!offset && !stem_up && note_group.get_value() != Value::Whole) || (offset && !stem_up && note_group.get_value() == Value::Whole)) {
					draw_symbol(note_head, draw_position + sf::Vector2f(second_interval_offset, 0), target, states, sf::Vector2f(1, 1), note_color);
				}
				else {
					draw_symbol(note_head, draw_position, target, states, sf::Vector2f(1, 1), note_color);
				}
			}

			// Draw ledger lines


			switch (note_group.get_value()) {
			case Value::Whole:
				draw_position.x += get_measure_width();
				break;
			case Value::Half:
				draw_position.x += get_measure_width() / 2;
				break;
			case Value::Quarter:
				draw_position.x += get_measure_width() / 4;
				break;
			case Value::Eight:
				draw_position.x += get_measure_width() / 8;
				break;
			}
		}
		else {
			// TODO: Rests
		}
	}
}

void SongRenderer::draw_key_signature(const Key& key, sf::Vector2f position, sf::RenderTarget& target, sf::RenderStates states) const
{
	constexpr wchar_t FLAT_ACCIDENTAL = 0xE260;
	constexpr wchar_t SHARP_ACCIDENTAL = 0xE262;
	float key_signature_gap{ get_music_size() / 4.0f };
	
	bool is_sharp{ false };
	int accidental_count{ 0 };

	switch (key) {
	case Key::CMajor:
	case Key::AMinor:
		accidental_count = 0;
		is_sharp = false;
		break;

	case Key::GMajor:
	case Key::EMinor:
		accidental_count = 1;
		is_sharp = true;
		break;
	case Key::DMajor:
	case Key::BMinor:
		accidental_count = 2;
		is_sharp = true;
		break;
	case Key::AMajor:
	case Key::FSharpMinor:
		accidental_count = 3;
		is_sharp = true;
		break;
	case Key::EMajor:
	case Key::CSharpMinor:
		accidental_count = 4;
		is_sharp = true;
		break;
	case Key::BMajor:
	case Key::GSharpMinor:
		accidental_count = 5;
		is_sharp = true;
		break;
	case Key::FSharpMajor:
	case Key::DSharpMinor:
		accidental_count = 6;
		is_sharp = true;
		break;
	case Key::CSharpMajor:
	case Key::ASharpMinor:
		accidental_count = 7;
		is_sharp = true;
		break;

	case Key::FMajor:
	case Key::DMinor:
		accidental_count = 1;
		is_sharp = false;
		break;
	case Key::BFlatMajor:
	case Key::GMinor:
		accidental_count = 2;
		is_sharp = false;
		break;
	case Key::EFlatMajor:
	case Key::CMinor:
		accidental_count = 3;
		is_sharp = false;
		break;
	case Key::AFlatMajor:
	case Key::FMinor:
		accidental_count = 4;
		is_sharp = false;
		break;
	case Key::DFlatMajor:
	case Key::BFlatMinor:
		accidental_count = 5;
		is_sharp = false;
		break;
	case Key::GFlatMajor:
	case Key::EFlatMinor:
		accidental_count = 6;
		is_sharp = false;
	case Key::CFlatMajor:
	case Key::AFlatMinor:
		accidental_count = 7;
		is_sharp = false;
		break;
	}

	if (is_sharp) {
		if (accidental_count > 0) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(0.0f, -get_vertical_pitch_separation() * 8), target, states);
		}
		if (accidental_count > 1) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap, -get_vertical_pitch_separation() * 5), target, states);
		}
		if (accidental_count > 2) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 2, -get_vertical_pitch_separation() * 9), target, states);
		}
		if (accidental_count > 3) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 3, -get_vertical_pitch_separation() * 6), target, states);
		}
		if (accidental_count > 4) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 4, -get_vertical_pitch_separation() * 3), target, states);
		}
		if (accidental_count > 5) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 5, -get_vertical_pitch_separation() * 7), target, states);
		}
		if (accidental_count > 6) {
			draw_symbol(SHARP_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 6, -get_vertical_pitch_separation() * 4), target, states);
		}
	}
	else {
		if (accidental_count > 0) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(0.0f, -get_vertical_pitch_separation() * 4), target, states);
		}
		if (accidental_count > 1) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap, -get_vertical_pitch_separation() * 7), target, states);
		}
		if (accidental_count > 2) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 2, -get_vertical_pitch_separation() * 3), target, states);
		}
		if (accidental_count > 3) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 3, -get_vertical_pitch_separation() * 6), target, states);
		}
		if (accidental_count > 4) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 4, -get_vertical_pitch_separation() * 2), target, states);
		}
		if (accidental_count > 5) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 5, -get_vertical_pitch_separation() * 5), target, states);
		}
		if (accidental_count > 6) {
			draw_symbol(FLAT_ACCIDENTAL, position + sf::Vector2f(key_signature_gap * 6, -get_vertical_pitch_separation() * 1), target, states);
		}
	}
}
