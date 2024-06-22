#include "TimeSignature.hpp"

TimeSignature::TimeSignature() {}

TimeSignature::TimeSignature(int numerator, int denominator)
	: m_numerator{ numerator }, m_denominator{ denominator } {}

TimeSignature::TimeSignature(const TimeSignature& source)
	: m_numerator{ source.m_numerator }, m_denominator{ source.m_denominator } {}

TimeSignature::~TimeSignature() {}

TimeSignature& TimeSignature::operator=(const TimeSignature& source) {
	m_numerator = source.m_numerator;
	m_denominator = source.m_denominator;

	return *this;
}

void TimeSignature::set(int numerator, int denominator) {
	m_numerator = numerator;
	m_denominator = denominator;
}

void TimeSignature::set_numerator(int numerator) {
	m_numerator = numerator;
}

void TimeSignature::set_denominator(int denominator) {
	m_denominator = denominator;
}

int TimeSignature::get_numerator() const {
	return m_numerator;
}

int TimeSignature::get_denominator() const {
	return m_denominator;
}

float TimeSignature::get_ratio() const {
	return static_cast<float>( m_numerator ) / static_cast<float>( m_denominator );
}
