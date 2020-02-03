#ifndef KGB_SINGLETONS_H
#define KGB_SINGLETONS_H


#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>

#include "ResourceManager.h"

namespace KGB {

    extern gf::Singleton<ResourceManager> gResourceManager;
    extern gf::Singleton<gf::MessageManager> gMessageManager;
    extern gf::Singleton<gf::Random> gRandom;

}



#endif // KGB_SINGLETONS_H
