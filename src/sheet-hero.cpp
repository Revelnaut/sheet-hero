﻿#include <iostream>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "SongRenderer.hpp"
#include "effolkronium/random.hpp"

using Random = effolkronium::random_static;

Song generate_random_song(int measures, Key key = Key::CMajor, int tempo = 120) {
	Song song{};
	song.set_key(key);
	song.set_tempo(tempo);
	for (int m = 0; m < measures; ++m) {
		Measure measure;

		while (measure.free_treble_space_in_eights() > 0) {
			Value largest_option{};

			if (measure.free_treble_space_in_eights() == 8) {
				largest_option = Value::Whole;
			}
			else if (measure.free_treble_space_in_eights() >= 4) {
				largest_option = Value::Half;
			}
			else if (measure.free_treble_space_in_eights() >= 2) {
				largest_option = Value::Quarter;
			}
			else {
				largest_option = Value::Eight;
			}

			Value value = static_cast<Value>(Random::get<int>(static_cast<int>(largest_option), static_cast<int>(Value::Eight)));
			NoteSet ns{ value };

			std::vector<int> staff_pitches{ 0, 1, 2, 3, 4, 5, 6 };
			Random::shuffle(staff_pitches);
			
			int note_count = Random::get(1, 3);
			for (int i = 0; i < note_count; ++i) {
				ns.add_note(Note{ static_cast<PitchClass>(staff_pitches[i]), static_cast<Accidental>(Random::get(0, 2)), 4});
			}
			measure.add_treble_note_set(ns);
		}

		while (measure.free_bass_space_in_eights() > 0) {
			Value largest_option = Value::Whole;

			if (measure.free_bass_space_in_eights() >= 4) {
				largest_option = Value::Half;
			}
			else if (measure.free_bass_space_in_eights() >= 2) {
				largest_option = Value::Quarter;
			}
			else {
				largest_option = Value::Eight;
			}

			Value value = static_cast<Value>(Random::get<int>(static_cast<int>(largest_option), static_cast<int>(Value::Eight)));
			NoteSet ns{ value };
			ns.add_note(Note{ static_cast<PitchClass>(Random::get(0, 6)), static_cast<Accidental>(Random::get(0, 2)), 3 });
			measure.add_bass_note_set(ns);
		}

		song.add_measure(measure);
	}
	return song;
}

int main()
{
	std::string window_title = "Sheet Hero";
	sf::Vector2u window_initial_size{ 1920, 1080 };
	sf::VideoMode windowed_mode{ window_initial_size.x, window_initial_size.y };
	sf::VideoMode fullscreen_mode{ sf::VideoMode::getFullscreenModes()[0] };
	bool is_fullscreen = false;

	sf::Color window_color{ 0xF0F0F0FF };

	sf::Font default_text_font{};
	default_text_font.loadFromFile("data/fonts/opensans.ttf");

	sf::RenderWindow window{ windowed_mode, window_title, sf::Style::Default };
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	float song_margin = 100.0;
	SongRenderer song_renderer{};
	song_renderer.setPosition(sf::Vector2f(song_margin, song_margin));
	song_renderer.set_max_width(window_initial_size.x - song_margin * 2);
	song_renderer.set_music_size(60);
	song_renderer.set_song(generate_random_song(8));
	song_renderer.set_music_color(sf::Color::Black);

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);

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
				else {
					if (event.key.code == sf::Keyboard::G) {
						std::cout << "Generating..." << std::endl;
						song_renderer.set_song(generate_random_song(32));
						std::cout << "Done!" << std::endl;
					}
				}
			}

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::Resized) {
				window.setView(sf::View{ sf::FloatRect{0.0f, 0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height)} });
				song_renderer.set_max_width(static_cast<float>(event.size.width) - song_margin * 2);
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