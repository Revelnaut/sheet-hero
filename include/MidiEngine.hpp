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
	typedef long long timestamp;

	struct InputDevice {
		std::unique_ptr<libremidi::midi_in> midi_in{};
		std::unique_ptr<libremidi::input_port> port_in{};
	};

	struct OutputDevice {
		std::unique_ptr<libremidi::midi_out> midi_out{};
		std::unique_ptr<libremidi::output_port> port_out{};
	};

	std::vector<InputDevice> m_input_devices{};
	std::vector<OutputDevice> m_output_devices{};
	std::unique_ptr<libremidi::observer> m_observer{};

	std::map<int, bool> m_pressed_note_states{};
	std::map<int, timestamp> m_pressed_note_timestamps{};

	void message_callback(libremidi::message&& message);
	void input_port_added_callback(const libremidi::input_port& port_id);
	void input_port_removed_callback(const libremidi::input_port& port_id);
	void output_port_added_callback(const libremidi::output_port& port_id);
	void output_port_removed_callback(const libremidi::output_port& port_id);
};