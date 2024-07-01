#pragma once

#include <SFML/System/Time.hpp>
#include "Song.hpp"

class SongPlayer {
public:
	void process(Song& song, const sf::Time& delta);

	void advance_playing_position(float delta);
	void set_playing_position(float position);
	float get_playing_position() const;
	
	void toggle_playing();
	void set_playing(bool is_playing);
	bool is_playing() const;

private:
	float m_playing_position{};
	bool m_is_playing{};
};