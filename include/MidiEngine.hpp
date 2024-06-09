#pragma once

#include "rtmidi/RtMidi.h"
#include <vector>
#include "MidiDevice.hpp"

class MidiEngine {
public:
	MidiEngine();
	~MidiEngine();

	void probe_midi_devices();
private:
	//std::unique_ptr<RtMidiIn> m_midi_in{};
	RtMidiIn m_midi_in{};
	std::vector<MidiDevice> m_devices{};
};