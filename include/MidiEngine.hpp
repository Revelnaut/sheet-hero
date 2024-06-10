#pragma once

#include <libremidi/libremidi.hpp>
#include <memory>
#include <vector>
#include <map>

class MidiEngine {
public:
	MidiEngine();
	~MidiEngine();

	void note_on(int pitch, int velocity);
	void note_off(int pitch, int velocity);

	std::vector<int> get_pressed_notes() const;
private:
	std::unique_ptr<libremidi::midi_in> m_midi_in;
	std::map<int, bool> m_pressed_note_states{};

	void message_callback(libremidi::message&& message);
};