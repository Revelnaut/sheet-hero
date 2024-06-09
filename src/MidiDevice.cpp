#include "MidiDevice.hpp"

MidiDevice::MidiDevice() {}

MidiDevice::MidiDevice(unsigned int port, std::string name)
	: m_port{ port }, m_name{ name } {}

MidiDevice::MidiDevice(const MidiDevice& source)
	: m_port{ source.m_port }, m_name{ source.m_name } {}

MidiDevice::~MidiDevice() {}

MidiDevice& MidiDevice::operator=(const MidiDevice& source) {
	m_port = source.m_port;
	m_name = source.m_name;

	return *this;
}

void MidiDevice::set_port(int port) {
	m_port = port;
}

void MidiDevice::set_name(const std::string& name) {
	m_name = name;
}

const unsigned int MidiDevice::get_port() const {
	return m_port;
}

const std::string& MidiDevice::get_name() const {
	return m_name;
}
