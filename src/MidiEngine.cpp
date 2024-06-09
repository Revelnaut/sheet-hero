#include "MidiEngine.hpp"
#include <iostream>
#include <libremidi/libremidi.hpp>

MidiEngine::MidiEngine() {
	auto message_callback = [](const libremidi::message& message) {
		auto nBytes = message.size();
		std::cout << "[ ";
		for (auto i = 0U; i < nBytes; i++)
			std::cout << std::hex << (int)message[i] << std::dec << " ";
		std::cout << "]";
		if (nBytes > 0)
			std::cout << " ; stamp = " << message.timestamp;
		std::cout << std::endl;
		};

	m_midi_in = std::make_unique<libremidi::midi_in>(libremidi::midi_in{ libremidi::input_configuration{.on_message = message_callback} });
	
	libremidi::observer obs;
	for (const libremidi::input_port& port : obs.get_input_ports()) {
		m_midi_in->open_port(port);
		break;
	}
}

MidiEngine::~MidiEngine() {}
