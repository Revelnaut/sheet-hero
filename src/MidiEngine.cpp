#include "MidiEngine.hpp"

MidiEngine::MidiEngine() {
	try {
		// Probe all midi ports for devices
		for (unsigned int i = 0; i < m_midi_in.getPortCount(); ++i) {
			std::cout << "Input port #" << i << ": " << m_midi_in.getPortName(i) << std::endl;
		}
	} catch (RtMidiError& error) {
		error.printMessage();
	}
}

MidiEngine::~MidiEngine() {}