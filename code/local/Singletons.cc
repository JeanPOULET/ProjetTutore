#include "Singletons.h"

namespace KGB {

  gf::Singleton<gf::ResourceManager> gResourceManager;
  gf::Singleton<gf::MessageManager> gMessageManager;
  gf::Singleton<gf::Random> gRandom;
  gf::Singleton<gf::TextureAtlas> gTextureAtlas;

}