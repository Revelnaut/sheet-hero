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
#include "PianoKeyboard.hpp"

using Random = effolkronium::random_static;

class App {
public:
	App();
	~App();

	int run();
	Song generate_random_song(int measures, Key key = Key::CMajor, int tempo = 120);
	void imgui_piano_widget(MidiEngine& midi_engine, const ImVec2& size);
};