#include "TimeSignature.hpp"

TimeSignature::TimeSignature() {}

TimeSignature::TimeSignature(int top, int bottom)
	: m_top{ top }, m_bottom{ bottom } {}

TimeSignature::TimeSignature(const TimeSignature& source)
	: m_top{ source.m_top }, m_bottom{ source.m_bottom } {}

TimeSignature::~TimeSignature() {}

TimeSignature& TimeSignature::operator=(const TimeSignature& source)
{
	m_top = source.m_top;
	m_bottom = source.m_bottom;

	return *this;
}

void TimeSignature::set(int top, int bottom) {
	m_top = top;
	m_bottom = bottom;
}

void TimeSignature::set_top(int top) {
	m_top = top;
}

void TimeSignature::set_bottom(int bottom) {
	m_bottom = bottom;
}

int TimeSignature::get_top() const {
	return m_top;
}

int TimeSignature::get_bottom() const {
	return m_bottom;
}
