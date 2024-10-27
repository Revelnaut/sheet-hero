#include "App.hpp"
#include "MusicalSymbol.hpp"
#include "SheetMusicSettings.hpp"
#include "GrandStaff.hpp"
#include <algorithm>

App::App() {
	click_sound.loadFromFile("data/audio/click.wav");
	click_player.setBuffer(click_sound);
}

App::~App() {}

int App::run() {
	create_window(false);
	ImGui::SFML::Init(window);

	generate_demo_song();

	sf::Clock delta_clock;
	while ( window.isOpen() ) {
		sf::Event event;
		while ( window.pollEvent(event) ) {
			ImGui::SFML::ProcessEvent(window, event);

			if ( event.type == sf::Event::KeyPressed ) {
				if ( event.key.alt && event.key.code == sf::Keyboard::Enter ) {
					toggle_fullscreen();
				} else {
					if ( event.key.code == sf::Keyboard::G ) {
						generate_demo_song();
					}

					if ( event.key.code == sf::Keyboard::Space ) {
						song_player.toggle_playing();
					}

					if ( event.key.code == sf::Keyboard::Tab ) {
						toggle_ui();
					}
				}
			}

			if ( event.type == sf::Event::Closed ) {
				window.close();
			}

			if ( event.type == sf::Event::Resized ) {
				update_view(static_cast<float>( event.size.width ), static_cast<float>( event.size.height ));
			}
		}

		sf::Time delta_time = delta_clock.restart();
		process(delta_time);

		window.clear(window_color);
		render();
		window.display();
	}
	ImGui::SFML::Shutdown();
	return EXIT_SUCCESS;
}

void App::generate_demo_song() {
	std::cout << "Generating..." << std::endl;
	song = generate_random_song(8, static_cast<Key>( Random::get(0, 29) ), 60);
	std::cout << "Done!" << std::endl;
	std::cout << "Song key: " << (int)song.get_key() << std::endl;
}

Song App::generate_random_song(int note_groups, Key key, int tempo) {
	Song song{};
	song.set_key(key);
	song.set_tempo(tempo);
	song.set_time_signature({ 3, 4 });

		GrandStaff grand_staff;
		Staff& treble = grand_staff.treble;
		Staff& bass = grand_staff.bass;

		for (int i = 0; i < note_groups; ++i) {
			Value value = static_cast<Value>( Random::get<int>(static_cast<int>( Value::Whole ), static_cast<int>( Value::Eight )) );
			NoteGroup ng{ value };

			std::vector<int> staff_pitches{ 1, 2, 3, 4, 5, 6, 7 };
			Random::shuffle(staff_pitches);

			//int note_count = Random::get(1, 5);
			int note_count = 1;
			for ( int i = 0; i < note_count; ++i ) {
				PitchClass pitch_class{ static_cast<PitchClass>( staff_pitches[i] ) };
				Accidental accidental{ song.get_scale().get_accidental(pitch_class) };

				// Small chance for a random accidental
				if ( Random::get<bool>(0.05) ) {
					accidental = DataUtility::int_to_accidental(Random::get(-1, 1));
				}
				int octave = Random::get(4, 5);
				ng.add_note(Note{ pitch_class, accidental, octave });
			}
			treble.add_note_group(ng);
		}

		for ( int i = 0; i < note_groups; ++i ) {
			Value value = static_cast<Value>( Random::get<int>(static_cast<int>( Value::Whole ), static_cast<int>( Value::Eight )) );
			NoteGroup ng{ value };

			std::vector<int> staff_pitches{ 1, 2, 3, 4, 5, 6, 7 };
			Random::shuffle(staff_pitches);

			//int note_count = Random::get(1, 3);
			int note_count = 1;
			for ( int i = 0; i < note_count; ++i ) {
				PitchClass pitch_class{ static_cast<PitchClass>( staff_pitches[i] ) };
				Accidental accidental{ song.get_scale().get_accidental(pitch_class) };

				// Small chance for a random accidental
				if ( Random::get<bool>(0.05) ) {
					accidental = DataUtility::int_to_accidental(Random::get(-1, 1));
				}

				int octave = Random::get(2, 3);
				ng.add_note(Note{ pitch_class, accidental, octave });
			}
			bass.add_note_group(ng);
		}

		song.set_grand_staff(grand_staff);
	return song;
}

void App::create_window(bool fullscreen) {
	std::string window_title = "Sheet Hero";
	sf::Vector2u window_initial_size{ 1920, 1080 };
	sf::VideoMode windowed_mode{ window_initial_size.x, window_initial_size.y };
	sf::VideoMode fullscreen_mode{ sf::VideoMode::getFullscreenModes()[0] };

	if ( fullscreen ) {
		window.create(fullscreen_mode, window_title, sf::Style::Fullscreen);
	} else {
		window.create(windowed_mode, window_title, sf::Style::Default);
	}
	window.setVerticalSyncEnabled(true);

	update_view(static_cast<float>( window.getSize().x ), static_cast<float>( window.getSize().y ));

	is_fullscreen = fullscreen;
}

void App::update_view(float width, float height) {
	window.setView(sf::View{ sf::FloatRect{ 0.0f, 0.0f, width, height } });
	song_renderer.set_bounds(sf::FloatRect{ song_margin, song_margin, width - song_margin * 2, height - song_margin * 2 });
}

void App::toggle_fullscreen() {
	create_window(!is_fullscreen);
}

void App::process(const sf::Time & delta) {
	ImGui::SFML::Update(window, delta);
	//imgui_show_interface();

	song_player.process(song, delta);
}

void App::render() {
	song_renderer.render(song, song_player.get_playing_position(), window);
	ImGui::SFML::Render(window);
}
