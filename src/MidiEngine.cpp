#include "MidiEngine.hpp"
#include <iostream>
#include <libremidi/libremidi.hpp>
#include <functional>

MidiEngine::MidiEngine() {
	using std::placeholders::_1;

	libremidi::input_configuration configuration{
		.on_message = std::bind(&MidiEngine::message_callback, this, _1)
	};

	m_midi_in = std::make_unique<libremidi::midi_in>(libremidi::midi_in{ configuration });
	
	for (int i = 0; i < 128; ++i) {
		m_pressed_note_states[i] = false;
	}

	libremidi::observer obs;
	libremidi::input_port port{};
	for (const libremidi::input_port& p : obs.get_input_ports()) {
		port = p;
		break;
	}
	m_midi_in->open_port(port);
}

MidiEngine::~MidiEngine() {}

void MidiEngine::note_on(int pitch, int velocity) {
	std::cout << "Pressed " << pitch << " at velocity " << velocity << std::endl;

	m_pressed_note_states[pitch] = true;
}

void MidiEngine::note_off(int pitch, int velocity) {
	std::cout << "Released " << pitch << " at velocity " << velocity << std::endl;

	m_pressed_note_states[pitch] = false;
}

std::vector<int> MidiEngine::get_pressed_notes() const
{
	std::vector<int> pressed_notes{};

	for (int i = 0; i < 128; ++i) {
		if (m_pressed_note_states.at(i) == true) {
			pressed_notes.push_back(i);
		}
	}

	return pressed_notes;
}

void MidiEngine::message_callback(libremidi::message&& message)
{
	if (message.size() > 0) {
		auto message_type = message[0];
		if (message_type >= 0x90 && message_type <= 0x9F) {
			note_on(message[1], message[2]);
		}
		else if (message_type >= 0x80 && message_type <= 0x8F) {
			note_off(message[1], message[2]);
		}
	}
}
