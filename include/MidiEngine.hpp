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

	void reset_notes();

	std::vector<int> get_pressed_notes() const;
	bool is_pressed(int index) const;
private:
	struct InputDevice {
		std::unique_ptr<libremidi::midi_in> m_midi_in{};
		std::unique_ptr<libremidi::input_port> m_port_in{};
	};

	std::unique_ptr<libremidi::observer> m_observer{};
	std::unique_ptr<libremidi::midi_in> m_midi_in{};
	std::map<int, bool> m_pressed_note_states{};

	std::vector<InputDevice> m_input_devices{};

	void message_callback(libremidi::message&& message);
	void input_port_added_callback(const libremidi::input_port& port_id);
	void input_port_removed_callback(const libremidi::input_port& port_id);
	void output_port_added_callback(const libremidi::output_port& port_id);
	void output_port_removed_callback(const libremidi::output_port& port_id);
};