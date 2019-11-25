#ifndef HOME_MUSIC_H
#define HOME_MUSIC_H

#include <SFML/Audio.hpp>

namespace home {

  class Musics {
  public:
    Musics();

    void toggleMute();

  private:
    sf::Music& m_music;
  };

}


#endif // HOME_MUSIC_H
