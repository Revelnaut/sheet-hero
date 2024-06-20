#pragma once

#include "MusicalData.hpp"
#include <unordered_map>
#include <string>

class Scale {
public:
	Scale();
	Scale(Key key);
	Scale(std::string name, int c, int d, int e, int f, int g, int a, int b);
	
	Scale(const Scale& source);
	Scale& operator=(const Scale& source);

	bool is_sharp(const PitchClass& pitch_class) const;
	bool is_flat(const PitchClass& pitch_class) const;
	bool is_natural(const PitchClass& pitch_class) const;

	void set_accidental(const PitchClass& pitch_class, const Accidental& accidental);
	const Accidental& get_accidental(const PitchClass& pitch_class) const;

	void create(std::string name, int c, int d, int e, int f, int g, int a, int b);
	void set_key(Key key);
private:
	std::unordered_map<PitchClass, Accidental> m_accidentals{};
	std::string m_name{};

	void set_accidentals(int c, int d, int e, int f, int g, int a, int b);
	void set_name(const std::string& name);
};
