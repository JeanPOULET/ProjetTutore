#ifndef KGB_PHYSICS_H
#define KGB_PHYSICS_H

#include <vector>

#include <Box2D/Box2D.h>

#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Model.h>
#include <gf/Polygon.h>
#include <gf/Tmx.h>

#include "BabyHero.h"
#include "Enemy.h"

namespace KGB {
  class Physics : public gf::Model {
  public:
    Physics(BabyHero& player, Enemy& policier);

    void update();

  private:
    b2World m_world;
    b2Body *m_body;
    BabyHero& m_baby;
    Enemy& m_vilain;
    b2Body *m_vilainBody;
  };

}

#endif // KGB_PHYSICS_H
