#pragma once

#include <libremidi/libremidi.hpp>
#include <memory>

class MidiEngine {
public:
	MidiEngine();
	~MidiEngine();
private:
	std::unique_ptr<libremidi::midi_in> m_midi_in;
};