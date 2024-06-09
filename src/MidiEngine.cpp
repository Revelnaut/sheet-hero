#include "MidiEngine.hpp"
#include <iostream>

MidiEngine::MidiEngine() {
	probe_midi_devices();
}

MidiEngine::~MidiEngine() {}

void MidiEngine::probe_midi_devices() {
	try {
		// Probe all midi ports for devices
		m_devices.clear();
		for (unsigned int i = 0; i < m_midi_in.getPortCount(); ++i) {
			m_devices.push_back(MidiDevice{ i, m_midi_in.getPortName(i) });
			std::cout << "Midi port #" << m_devices.back().get_port() << ": " << m_devices.back().get_name() << std::endl;
		}
	}
	catch (RtMidiError& error) {
		error.printMessage();
	}
}
