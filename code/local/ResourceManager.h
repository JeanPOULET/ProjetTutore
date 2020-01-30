
#ifndef KGB_RESOURCEMANAGER_H
#define KGB_RESOURCEMANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace KGB {

  class ResourceManager : public gf::ResourceManager {
    public:
      ResourceManager();
      sf::SoundBuffer& getSound(const gf::Path &path);
      sf::Music& getMusic(const gf::Path &path);

    private:
      gf::ResourceCache<sf::SoundBuffer> m_sounds;
      gf::ResourceCache<sf::Music> m_musics;
    };

}

#endif // KGB_RESOURCEMANAGER_H
