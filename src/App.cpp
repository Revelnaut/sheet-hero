#include "App.hpp"

App::App()
{
}

App::~App()
{
}

int App::run()
{
	MidiEngine midi_engine{};

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
	SongRenderer song_renderer{ generate_random_song(1) };
	song_renderer.setPosition(sf::Vector2f(song_margin, song_margin));
	song_renderer.set_max_width(window_initial_size.x - song_margin * 2);
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

					if (event.key.code == sf::Keyboard::M) {
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

		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Midi")) {
			imgui_piano_widget(midi_engine, ImVec2(800, 100));
			ImGui::Text("Pressed midi keys:");
			for (auto pitch : midi_engine.get_pressed_notes()) {
				ImGui::Text("%i", pitch);
			}

			ImGui::End();
		}

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
				ng.add_note(Note{ static_cast<PitchClass>(staff_pitches[i]), static_cast<Accidental>(Random::get(0, 2)), 4 });
			}
			measure.add_treble_note_group(ng);
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
			NoteGroup ns{ value };
			ns.add_note(Note{ static_cast<PitchClass>(Random::get(0, 6)), static_cast<Accidental>(Random::get(0, 2)), 3 });
			measure.add_bass_note_group(ns);
		}

		song.add_measure(measure);
	}
	return song;
}

void App::imgui_piano_widget(MidiEngine& midi_engine, const ImVec2& size) {
	ImVec2 white_key_size = ImVec2(size.x / 52.0f, size.y);
	ImVec2 black_key_size = ImVec2(white_key_size.x * 0.6f, white_key_size.y * 0.7f);
	ImVec2 start_position = ImGui::GetCursorScreenPos();
	ImVec2 current_position = start_position;

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);

	for (int i = 0; i < 88; ++i) {
		int key_in_octave = (i + 9) % 12;
		int midi_pitch = i + 21;
		switch (key_in_octave) {
		case 0:
		case 2:
		case 4:
		case 5:
		case 7:
		case 9:
		case 11:
			if (midi_engine.is_pressed(midi_pitch)) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
			ImGui::SetNextItemAllowOverlap();
			std::string id = "##piano_key_" + std::to_string(i);
			ImGui::Button(id.c_str(), white_key_size);
			if (ImGui::IsItemClicked()) {
				midi_engine.note_on(midi_pitch, 128);
			}
			if (ImGui::IsItemDeactivated()) {
				midi_engine.note_off(midi_pitch, 0);
			}
			current_position.x += white_key_size.x;
			ImGui::SetCursorScreenPos(current_position);
			ImGui::PopStyleColor(3);
			break;
		}
	}

	current_position = start_position;
	current_position.x -= black_key_size.x / 2.0f;

	for (int i = 0; i < 88; ++i) {
		int key_in_octave = (i + 9) % 12;
		int midi_pitch = i + 21;
		switch (key_in_octave) {
		case 0:
		case 2:
		case 4:
		case 5:
		case 7:
		case 9:
		case 11:
			current_position.x += white_key_size.x;
			break;
		case 1:
		case 3:
		case 6:
		case 8:
		case 10:
			if (midi_engine.is_pressed(midi_pitch)) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			ImGui::SetCursorScreenPos(current_position);
			std::string id = "##piano_key_" + std::to_string(i);
			ImGui::Button(id.c_str(), black_key_size);
			if (ImGui::IsItemClicked()) {
				midi_engine.note_on(midi_pitch, 128);
			}
			if (ImGui::IsItemDeactivated()) {
				midi_engine.note_off(midi_pitch, 0);
			}
			ImGui::PopStyleColor(3);
			break;
		}
	}
	current_position = start_position;
	current_position.y += white_key_size.y + 10;
	ImGui::SetCursorScreenPos(current_position);

	ImGui::PopStyleVar(1);
}