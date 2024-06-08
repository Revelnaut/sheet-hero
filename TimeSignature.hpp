#pragma once

class TimeSignature {
public:
	TimeSignature();
	TimeSignature(int top, int bottom);
	TimeSignature(const TimeSignature& source);
	~TimeSignature();

	TimeSignature& operator=(const TimeSignature& source);

	void set(int top, int bottom);
	void set_top(int top);
	void set_bottom(int bottom);

	int get_top() const;
	int get_bottom() const;

private:
	int m_top{ 4 };
	int m_bottom{ 4 };
};