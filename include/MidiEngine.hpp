#pragma once

#include <libremidi/libremidi.hpp>
#include <memory>

class MidiEngine {
public:
	MidiEngine();
	~MidiEngine();

	void note_on(int pitch, int velocity);
	void note_off(int pitch, int velocity);
private:
	std::unique_ptr<libremidi::midi_in> m_midi_in;
};