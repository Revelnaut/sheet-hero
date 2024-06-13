#include "App.hpp"

App::App()
{
	default_text_font.loadFromFile("data/fonts/opensans.ttf");
}

App::~App()
{
}

int App::run()
{
	create_window(false);
	ImGui::SFML::Init(window);

	song_renderer.set_song(generate_random_song(32));
	song_renderer.setPosition(sf::Vector2f(song_margin, song_margin));
	song_renderer.set_max_width(window.getSize().x - song_margin * 2);
	song_renderer.set_music_size(50);
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
					toggle_fullscreen();
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
				update_view(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		//ImGui::ShowDemoWindow();
		//imgui_show_interface();

		window.clear(window_color);
		window.draw(song_renderer);
		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();
	return EXIT_SUCCESS;
}

Song App::generate_random_song(int measures, Key key, int tempo) {
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
			NoteGroup ng{ value };

			std::vector<int> staff_pitches{ 0, 1, 2, 3, 4, 5, 6 };
			Random::shuffle(staff_pitches);

			int note_count = Random::get(1, 3);
			for (int i = 0; i < note_count; ++i) {
				ng.add_note(Note{ static_cast<PitchClass>(staff_pitches[i]), static_cast<Accidental>(Random::get(0, 2)), Random::get(4, 5)});
			}
			measure.add_treble_note_group(ng);
		}

		while (measure.free_bass_space_in_eights() > 0) {
			Value largest_option{};
			
			if (measure.free_bass_space_in_eights() == 8) {
				largest_option = Value::Whole;
			}
			else if (measure.free_bass_space_in_eights() >= 4) {
				largest_option = Value::Half;
			}
			else if (measure.free_bass_space_in_eights() >= 2) {
				largest_option = Value::Quarter;
			}
			else {
				largest_option = Value::Eight;
			}

			Value value = static_cast<Value>(Random::get<int>(static_cast<int>(largest_option), static_cast<int>(Value::Eight)));
			NoteGroup ng{ value };
			
			std::vector<int> staff_pitches{ 0, 1, 2, 3, 4, 5, 6 };
			Random::shuffle(staff_pitches);

			int note_count = Random::get(1, 3);
			for (int i = 0; i < note_count; ++i) {
				ng.add_note(Note{ static_cast<PitchClass>(staff_pitches[i]), static_cast<Accidental>(Random::get(0, 2)), Random::get(2, 3) });
			}
			measure.add_bass_note_group(ng);
		}

		song.add_measure(measure);
	}
	return song;
}



void App::create_window(bool fullscreen)
{
	std::string window_title = "Sheet Hero";
	sf::Vector2u window_initial_size{ 1920, 1080 };
	sf::VideoMode windowed_mode{ window_initial_size.x, window_initial_size.y };
	sf::VideoMode fullscreen_mode{ sf::VideoMode::getFullscreenModes()[0] };

	if (fullscreen) {
		window.create(fullscreen_mode, window_title, sf::Style::Fullscreen);
	}
	else {
		window.create(windowed_mode, window_title, sf::Style::Default);
	}
	window.setVerticalSyncEnabled(true);

	update_view(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

	is_fullscreen = fullscreen;
}

void App::update_view(float width, float height) {
	window.setView(sf::View{ sf::FloatRect{ 0.0f, 0.0f, width, height } });
	song_renderer.set_max_width(width - song_margin * 2);
}

void App::toggle_fullscreen()
{
	create_window(!is_fullscreen);
}