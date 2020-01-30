#ifndef KGB_BACKGROUND_MUSIC_H
#define KGB_BACKGROUND_MUSIC_H


#include <SFML/Audio.hpp>
#include "Singletons.h"

namespace KGB {

  class BackgroundMusic {
    public:
        BackgroundMusic();

        void toggleMute();

    private:
        sf::Music& m_music;
    };

}

#endif // KGB_BACKGROUND_MUSIC_H