#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <imgui.h>
#include <imgui-SFML.h>

#include "SongRenderer.hpp"

int main()
{
	std::string window_title = "Sheet Hero";
	sf::Vector2u window_initial_size{ 1920, 1080 };
	sf::VideoMode windowed_mode{ window_initial_size.x, window_initial_size.y };
	sf::VideoMode fullscreen_mode{ sf::VideoMode::getFullscreenModes()[0] };
	bool is_fullscreen = false;

	sf::Color window_color{ sf::Color::White };
	sf::Color music_color{ sf::Color::Black };

	sf::Font default_text_font{};
	default_text_font.loadFromFile("data/fonts/opensans.ttf");

	sf::RenderWindow window{ windowed_mode, window_title, sf::Style::Default };
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	NoteSet note_set_1{ Value::Half };
	note_set_1.add_note(Note{ PitchClass::C, Accidental::Natural, 4 });
	note_set_1.add_note(Note{ PitchClass::E, Accidental::Natural, 4 });
	note_set_1.add_note(Note{ PitchClass::G, Accidental::Natural, 4 });

	NoteSet note_set_2{ Value::Half };
	note_set_2.add_note(Note{ PitchClass::F, Accidental::Natural, 4 });
	note_set_2.add_note(Note{ PitchClass::A, Accidental::Sharp, 4 });
	note_set_2.add_note(Note{ PitchClass::C, Accidental::Natural, 5 });

	NoteSet note_set_3{ Value::Eight };
	note_set_3.add_note(Note{ PitchClass::C, Accidental::Natural, 5 });

	Measure measure_1{};
	measure_1.add_treble_note_set(note_set_1);
	measure_1.add_treble_note_set(note_set_2);

	Measure measure_2{};
	note_set_1.set_value(Value::Whole);
	measure_2.add_treble_note_set(note_set_1);
	measure_2.add_bass_note_set(note_set_2);
	measure_2.add_bass_note_set(note_set_2);

	Measure measure_3{};
	for (int i = 0; i < 8; ++i) {
		measure_3.add_treble_note_set(note_set_3);
	}

	Song song{};
	for (int i = 0; i < 4; ++i) {
		song.add_measure(measure_1);
		song.add_measure(measure_2);
		song.add_measure(measure_3);
	}

	float song_margin = 100.0;
	SongRenderer song_renderer{};
	song_renderer.setPosition(sf::Vector2f(song_margin, song_margin));
	song_renderer.set_max_width(window_initial_size.x - song_margin * 2);
	song_renderer.set_music_size(40);
	song_renderer.set_song(song);

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.alt && event.key.code == sf::Keyboard::Enter) {
					if (is_fullscreen) {
						window.create(windowed_mode, window_title, sf::Style::Default);
						song_renderer.set_max_width(static_cast<float>(window.getSize().x) - song_margin * 2);
						is_fullscreen = false;
					}
					else {
						windowed_mode = sf::VideoMode{ window.getSize().x, window.getSize().y };
						window.create(fullscreen_mode, window_title, sf::Style::Fullscreen);
						song_renderer.set_max_width(static_cast<float>(window.getSize().x) - song_margin * 2);
						is_fullscreen = true;
					}
				}
			}
			else {
				ImGui::SFML::ProcessEvent(window, event);

				if (event.type == sf::Event::Closed) {
					window.close();
				}

				if (event.type == sf::Event::Resized) {
					window.setView(sf::View{ sf::FloatRect{0.0f, 0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height)} });
					song_renderer.set_max_width(static_cast<float>(event.size.width) - song_margin * 2);
				}
			}
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		//ImGui::ShowDemoWindow();

		window.clear(window_color);
		window.draw(song_renderer);
		ImGui::SFML::Render(window);
		window.display();
	}
	ImGui::SFML::Shutdown();
	return EXIT_SUCCESS;
}
