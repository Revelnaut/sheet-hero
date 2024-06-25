#pragma once

#include <iostream>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "effolkronium/random.hpp"
using Random = effolkronium::random_static;

#include "MidiEngine.hpp"
#include "Song.hpp"
#include "SongRenderer.hpp"
#include "SongGenerator.hpp"

class App {
public:
	sf::Color window_color{ 0xF0F0F0FF };
	sf::RenderWindow window{};

	sf::SoundBuffer click_sound{};
	sf::Sound click_player{};
	bool play_click{ true };

	MidiEngine midi_engine{};
	Song song{};
	SongGenerator song_generator{};
	SongRenderer song_renderer{};
	float song_margin{ 100.0f };

	bool song_is_playing{ false };
	float song_position{};
	
	bool is_fullscreen{ false };
	bool show_ui{ true };

	App();
	~App();

	int run();

	void generate_demo_song();
	Song generate_random_song(int measures, Key key = Key::CMajor, int tempo = 120);

	void create_window(bool fullscreen);
	void update_view(float width, float height);
	void toggle_fullscreen();

	void process(const sf::Time & delta);
	void render();
	void toggle_playing();
	
	void toggle_ui();
	void imgui_show_interface();
	void imgui_midi_window();
	void imgui_piano_widget(MidiEngine& midi_engine, const ImVec2& size);
	void imgui_settings_window();
	void imgui_song_menu();
};