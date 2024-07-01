#include "SongPlayer.hpp"
#include <algorithm>

#include <iostream>

void SongPlayer::process(Song& song, const sf::Time& delta) {
	if ( m_is_playing ) {
		float beats = static_cast<float>( song.get_beat_count() );
		float beats_per_second = static_cast<float>( song.get_tempo() ) / 60.0f;
		float advance_per_second = beats_per_second / beats;

		advance_playing_position(advance_per_second * delta.asSeconds());
	}
}

void SongPlayer::advance_playing_position(float delta) {
	set_playing_position(get_playing_position() + delta);
}

void SongPlayer::set_playing_position(float position) {

	// End playback when the song reaches its end
	if ( position >= 1.0f ) {
		m_is_playing = false;
		m_playing_position = 0.0f;
	} else {
		m_playing_position = std::clamp(position, 0.0f, 1.0f);
	}
}

float SongPlayer::get_playing_position() const {
	return m_playing_position;
}

void SongPlayer::toggle_playing() {
	m_is_playing = !m_is_playing;
}

void SongPlayer::set_playing(bool is_playing) {
	m_is_playing = is_playing;
}

bool SongPlayer::is_playing() const {
	return m_is_playing;
}
