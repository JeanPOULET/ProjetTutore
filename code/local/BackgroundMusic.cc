#include "BackgroundMusic.h"
#include "Singletons.h"
namespace KGB {

	static constexpr float DefaultMusicVolume = 100.0f;

	BackgroundMusic::BackgroundMusic()
	: m_music(gResourceManager().getMusic("sounds/theme.ogg"))
	{
		m_music.setLoop(true);
		m_music.setVolume(DefaultMusicVolume);
		m_music.play();
	}

	void BackgroundMusic::toggleMute() {
		if (m_music.getStatus() == sf::SoundSource::Playing) {
			m_music.pause();
		} else {
			m_music.play();
		}
	}

}

