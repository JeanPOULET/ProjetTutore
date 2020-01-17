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
#include "Debug.h"


namespace KGB {
  class Physics : public gf::Model {
  public:
    Physics(BabyHero& player, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4);

    void update();

  private:
    b2World m_world;
    b2Body *m_body;
    BabyHero& m_baby;
    Enemy& m_vilain1;
    b2Body *m_vilainBody1;
    Enemy& m_vilain2;
    b2Body *m_vilainBody2;
    Enemy& m_vilain3;
    b2Body *m_vilainBody3;
    Enemy& m_vilain4;
    b2Body *m_vilainBody4;
  };

}

#endif // KGB_PHYSICS_H
