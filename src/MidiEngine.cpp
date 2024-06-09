#include "MidiEngine.hpp"
#include <iostream>
#include <libremidi/libremidi.hpp>

MidiEngine::MidiEngine() {
	auto message_callback = [this](const libremidi::message& message) {
		if (message.size() > 0) {
			auto message_type = message[0];
			if (message_type >= 0x90 && message_type <= 0x9F) {
				note_on(message[1], message[2]);
			}
			else if (message_type >= 0x80 && message_type <= 0x8F) {
				note_off(message[1], message[2]);
			}
		}
		};

	m_midi_in = std::make_unique<libremidi::midi_in>(libremidi::midi_in{ libremidi::input_configuration{.on_message = message_callback} });
	
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
	std::cout << "Pressed " << pitch << std::endl;
}

void MidiEngine::note_off(int pitch, int velocity) {
	std::cout << "Released " << pitch << std::endl;
}
