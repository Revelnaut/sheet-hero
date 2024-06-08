#include <iostream>
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
		
		Value value = static_cast<Value>(Random::get<int>(static_cast<int>(Value::Whole), static_cast<int>(Value::Eight)));
		NoteSet ns{ value };
		ns.add_note(Note{ PitchClass::C, Accidental::Natural, 4 });

		measure.add_treble_note_set(ns);

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

	sf::Color window_color{ sf::Color::White };
	sf::Color music_color{ sf::Color::Black };

	sf::Font default_text_font{};
	default_text_font.loadFromFile("data/fonts/opensans.ttf");

	sf::RenderWindow window{ windowed_mode, window_title, sf::Style::Default };
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	Song song = generate_random_song(4);

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
