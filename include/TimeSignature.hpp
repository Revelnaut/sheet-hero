#pragma once

class TimeSignature {
public:
	TimeSignature();
	TimeSignature(int numerator, int denominator);
	TimeSignature(const TimeSignature& source);
	~TimeSignature();

	TimeSignature& operator=(const TimeSignature& source);

	void set(int numerator, int denominator);
	void set_numerator(int numerator);
	void set_denominator(int denominator);

	int get_numerator() const;
	int get_denominator() const;

	float get_ratio() const;
private:
	int m_numerator{ 4 };
	int m_denominator{ 4 };
};