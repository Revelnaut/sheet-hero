#include "App.hpp"

namespace ImGui {

	/* Helper function created by Jakob Maier
	 * Source: https://www.jakobmaier.at/posts/imgui-slider-with-getter-and-setters/
	 */
	template <typename T, typename Getter, typename Setter>
	void SliderFloat(const char* label, T* t, Getter getter, Setter setter, float min, float max,
					 const char* format = "%.3f") {
		float current = ( t->*getter )( );
		float new_value = current;

		ImGui::SliderFloat(label, &new_value, min, max, format);

		if ( current != new_value ) {
			( t->*setter )( new_value );
		}
	}

	template <typename T, typename Getter, typename Setter>
	void SliderInt(const char* label, T* t, Getter getter, Setter setter, int min, int max,
					  const char* format = "%d") {
		int current = ( t->*getter )( );
		int new_value = current;

		ImGui::SliderInt(label, &new_value, min, max, format);

		if ( current != new_value ) {
			( t->*setter )( new_value );
		}
	}
}

void App::toggle_ui() {
	show_ui = !show_ui;
}

void App::imgui_show_interface() {
	if ( show_ui ) {
		ImGui::ShowDemoWindow();
		imgui_midi_window();
		imgui_settings_window();
		imgui_song_menu();
	}
}

void App::imgui_midi_window() {
	if ( ImGui::Begin("Midi") ) {
		imgui_piano_widget(midi_engine, ImVec2(800, 100));
		ImGui::Text("Pressed midi keys:");
		for ( auto pitch : midi_engine.get_pressed_notes() ) {
			ImGui::Text("%i", pitch);
		}

		ImGui::End();
	}
}

void App::imgui_piano_widget(MidiEngine& midi_engine, const ImVec2& size) {
	ImVec2 white_key_size = ImVec2(size.x / 52.0f, size.y);
	ImVec2 black_key_size = ImVec2(white_key_size.x * 0.6f, white_key_size.y * 0.7f);
	ImVec2 start_position = ImGui::GetCursorScreenPos();
	ImVec2 current_position = start_position;

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);

	for ( int i = 0; i < 88; ++i ) {
		int key_in_octave = ( i + 9 ) % 12;
		int midi_pitch = i + 21;
		switch ( key_in_octave ) {
		case 0:
		case 2:
		case 4:
		case 5:
		case 7:
		case 9:
		case 11:
			if ( midi_engine.is_pressed(midi_pitch) ) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
			ImGui::SetNextItemAllowOverlap();
			std::string id = "##piano_key_" + std::to_string(i);
			ImGui::Button(id.c_str(), white_key_size);
			if ( ImGui::IsItemClicked() ) {
				midi_engine.note_on(midi_pitch, 128);
			}
			if ( ImGui::IsItemDeactivated() ) {
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

	for ( int i = 0; i < 88; ++i ) {
		int key_in_octave = ( i + 9 ) % 12;
		int midi_pitch = i + 21;
		switch ( key_in_octave ) {
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
			if ( midi_engine.is_pressed(midi_pitch) ) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			ImGui::SetCursorScreenPos(current_position);
			std::string id = "##piano_key_" + std::to_string(i);
			ImGui::Button(id.c_str(), black_key_size);
			if ( ImGui::IsItemClicked() ) {
				midi_engine.note_on(midi_pitch, 128);
			}
			if ( ImGui::IsItemDeactivated() ) {
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

void App::imgui_settings_window() {
	if ( ImGui::Begin("Settings") ) {
		ImGui::SliderFloat("Brace spacing", &song_renderer.get_settings().grand_staff_brace_spacing_scale, 0.0f, 4.0f, "%.1f");
		ImGui::SliderFloat("Clef spacing", &song_renderer.get_settings().clef_spacing_scale, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Time signature spacing", &song_renderer.get_settings().time_signature_spacing_scale, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Key signature spacing", &song_renderer.get_settings().key_signature_spacing_scale, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Key signature accidental spacing", &song_renderer.get_settings().key_signature_accidental_spacing_scale, 0.0f, 4.0f, "%.1f");
		ImGui::SliderFloat("Bar width", &song_renderer.get_settings().bar_width_scale, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Measure width", &song_renderer.get_settings().measure_width_scale, 0.0f, 4.0f, "%.1f");
		ImGui::SliderFloat("First measure offset", &song_renderer.get_settings().first_measure_offset_scale, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Staff spacing", &song_renderer.get_settings().staff_spacing_scale, 0.0f, 4.0f, "%.1f");
		ImGui::SliderFloat("Grand staff spacing", &song_renderer.get_settings().grand_staff_spacing_scale, 0.0f, 4.0f, "%.1f");
		ImGui::SliderFloat("Line thickness", &song_renderer.get_settings().line_thickness_scale, 1.0f, 8.0f, "%.0f");
		ImGui::End();
	}
}

void App::imgui_song_menu() {
	if ( ImGui::Begin("Song") ) {
		ImGui::SliderFloat("Song position", &song_position, 0.0f, 0.99999f, "%.5f");

		auto& note_group = song.get_note_group_at_position(song_renderer.get_playing_position(), true);

		for ( auto& note : note_group.get_notes() ) {
			ImGui::Text(note.to_string().c_str());
		}

		ImGui::End();
	}
}
