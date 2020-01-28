#ifndef KGB_PHYSICS_H
#define KGB_PHYSICS_H

#include <vector>

#include <Box2D/Box2D.h>

#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Model.h>
#include <gf/Polygon.h>
#include <gf/Tmx.h>
#include <stdio.h>

#include <set>


#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Polyline.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "BabyHero.h"
#include "Enemy.h"
#include "Debug.h"
#include "Square.h"

namespace KGB {
	class Physics : public gf::Model, public b2ContactListener {
	public:
		Physics(const gf::TmxLayers& layers, BabyHero& player, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4, Enemy& policier5);
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
		Enemy& m_vilain5;
		b2Body *m_vilainBody5;
	};
}

#endif // KGB_PHYSICS_H
