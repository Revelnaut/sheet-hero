#include "MidiEngine.hpp"
#include <iostream>
#include <libremidi/libremidi.hpp>
#include <functional>

MidiEngine::MidiEngine() {
	using std::placeholders::_1;

	libremidi::input_configuration input_configuration{
		.on_message = std::bind(&MidiEngine::message_callback, this, _1)
	};

	m_midi_in = std::make_unique<libremidi::midi_in>(libremidi::midi_in{ input_configuration });
	
	for (int i = 0; i < 128; ++i) {
		m_pressed_note_states[i] = false;
	}

	libremidi::observer_configuration observer_configuration{
		.input_added = std::bind(&MidiEngine::input_port_added_callback, this, _1),
		.input_removed = std::bind(&MidiEngine::input_port_removed_callback, this, _1),
		.output_added = std::bind(&MidiEngine::output_port_added_callback, this, _1),
		.output_removed = std::bind(&MidiEngine::output_port_removed_callback, this, _1)
	};
	
	m_observer = std::make_unique<libremidi::observer>(libremidi::observer{observer_configuration});
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

bool MidiEngine::is_pressed(int index) const
{
	return m_pressed_note_states.at(index) == true;
}

void MidiEngine::message_callback(libremidi::message&& message)
{
	if (message.size() > 0) {
		auto message_type = message[0];
		if (message_type >= 0x90 && message_type <= 0x9F) {
			if (message[2] == 0) { // Velocity is 0
				note_off(message[1], message[2]);
			}
			else {
				note_on(message[1], message[2]);
			}
		}
		else if (message_type >= 0x80 && message_type <= 0x8F) {
			note_off(message[1], message[2]);
		}
	}
}

static void print_port_info(const libremidi::port_information& port_info) {
	std::cout << "Port: " << port_info.port << std::endl;
	std::cout << "Port name: " << port_info.port_name << std::endl;
	std::cout << "Device name: " << port_info.device_name << std::endl;
	std::cout << "Display name: " << port_info.display_name << std::endl;
	std::cout << "Manufacturer: " << port_info.manufacturer << std::endl;
	std::cout << std::endl;
}

void MidiEngine::input_port_added_callback(const libremidi::input_port& port_id) {
	std::cout << "Added input device:" << std::endl;
	print_port_info(port_id);

	if (m_midi_in->is_port_open() == false) {
		m_midi_in->open_port(port_id);
	}
}

void MidiEngine::input_port_removed_callback(const libremidi::input_port& port_id) {
	std::cout << "Removed input device:" << std::endl;
	print_port_info(port_id);
}

void MidiEngine::output_port_added_callback(const libremidi::output_port& port_id) {
	std::cout << "Added output device:" << std::endl;
	print_port_info(port_id);
}

void MidiEngine::output_port_removed_callback(const libremidi::output_port& port_id) {
	std::cout << "Removed output device:" << std::endl;
	print_port_info(port_id);
}
