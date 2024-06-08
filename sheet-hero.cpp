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
	sf::Vector2u window_initial_size{ 1920, 1080 };
	sf::Color window_color{ sf::Color::White };
	sf::Color music_color{ sf::Color::Black };

	sf::Font default_text_font{};
	default_text_font.loadFromFile("data/fonts/opensans.ttf");

	sf::RenderWindow window{ sf::VideoMode{window_initial_size.x, window_initial_size.y}, "Sheet Hero", sf::Style::Default };
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	SongRenderer song_renderer{};
	song_renderer.setPosition(sf::Vector2f(100, 100));

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::Resized) {
				window.setView(sf::View{ sf::FloatRect{0.0f, 0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height)} });
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
