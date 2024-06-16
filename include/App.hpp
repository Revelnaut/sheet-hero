#pragma once

#include <iostream>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "MidiEngine.hpp"
#include <libremidi/libremidi.hpp>
#include "SongRenderer.hpp"
#include "effolkronium/random.hpp"

using Random = effolkronium::random_static;

class App {
public:
	MidiEngine midi_engine{};
	sf::Color window_color{ 0xF0F0F0FF };
	sf::Font default_text_font{};
	sf::RenderWindow window{};
	bool is_fullscreen{ false };
	SongRenderer song_renderer{};
	float song_margin{ 100.0f };

	App();
	~App();

	int run();
	
	void generate_demo_song();
	Song generate_random_song(int measures, Key key = Key::CMajor, int tempo = 120);

	void create_window(bool fullscreen);
	void update_view(float width, float height);
	void toggle_fullscreen();

	void imgui_show_interface();
	void imgui_midi_window();
	void imgui_piano_widget(MidiEngine& midi_engine, const ImVec2& size);
	void imgui_settings_window();
};