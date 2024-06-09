#pragma once

#include <string>

class MidiDevice {
public:
	MidiDevice();
	MidiDevice(unsigned int port, std::string name);
	MidiDevice(const MidiDevice& source);
	~MidiDevice();

	MidiDevice& operator=(const MidiDevice& source);

	void set_port(int port);
	void set_name(const std::string& name);

	const unsigned int get_port() const;
	const std::string& get_name() const;
private:
	unsigned int m_port{ 0 };
	std::string m_name{};
};