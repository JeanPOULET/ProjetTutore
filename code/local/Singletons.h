#ifndef KBG_SINGLETONS_H
#define KBG_SINGLETONS_H


#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/Singleton.h>
#include <gf/TextureAtlas.h>


namespace KGB {

  extern gf::Singleton<gf::ResourceManager> gResourceManager;
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  extern gf::Singleton<gf::Random> gRandom;
  extern gf::Singleton<gf::TextureAtlas> gTextureAtlas;

}



#endif // KGB_SINGLETONS_H
