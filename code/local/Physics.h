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
#include <string>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Polyline.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>
#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Polygon.h>
#include <gf/Vector.h>

#include "BabyHero.h"
#include "Enemy.h"
#include "Bonus.h"
#include "b2dContactListener.h"
#include "DataType.h"

namespace KGB {

	class Enemy;
	class BabyHero;
	class Bonus;

	class Physics : public gf::Model {
		
		public:
			Physics(const gf::TmxLayers& layers, BabyHero& player, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4, Enemy& policier5, Bonus& bon1, Bonus& bon2, Bonus& bon3);
			void setPhysicCone(int vilain);
			
			static float getPhysicScale();
			static b2Vec2 fromVec(gf::Vector2f vec);
        	static gf::Vector2f toVec(b2Vec2 vec);
		
			void update();
			b2World& getWorld();

		private:
			b2World m_world;
			BabyHero& m_baby;
			
			Enemy& m_vilain1;
			Enemy& m_vilain2;
			Enemy& m_vilain3;
			Enemy& m_vilain4;
			Enemy& m_vilain5;
		
			Bonus& m_bonus1;
			Bonus& m_bonus2;
			Bonus& m_bonus3;
	};

	struct PhysicsState;

	class PhysicsDebugger : public gf::Entity {
		public:
			
			PhysicsDebugger(b2World& world)
			: gf::Entity(10000)
			, m_state(world)
			{

			}
			void setDebug(bool debug);
			virtual void render(gf::RenderTarget& target);
		private:
      		b2World& m_state;

			struct Polygon {
				gf::Polygon shape;
				gf::Color4f color;
			};

			struct Circle {
				gf::CircF shape;
				gf::Color4f color;
			};

			struct SolidCircle {
				gf::CircF shape;
				gf::Vector2f axis;
				gf::Color4f color;
			};

			struct Segment {
				gf::Vector2f p1;
				gf::Vector2f p2;
				gf::Color4f color;
			};

			struct Transform {
				gf::Vector2f position;
				gf::Vector2f xAxis;
				gf::Vector2f yAxis;
			};

			struct Point {
				gf::Vector2f position;
				float size;
				gf::Color4f color;
			};

		struct PhysicsDraw : public b2Draw {
		public:
			PhysicsDraw();

			/// Draw a closed polygon provided in CCW order.
			virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

			/// Draw a solid closed polygon provided in CCW order.
			virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

			/// Draw a circle.
			virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

			/// Draw a solid circle.
			virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

			/// Draw a line segment.
			virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

			/// Draw a transform. Choose your own length scale.
			/// @param xf a transform.
			virtual void DrawTransform(const b2Transform& xf) override;

			/// Draw a point.
			virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

			std::vector<Polygon> polygons;
			std::vector<Polygon> solidPolygons;
			std::vector<Circle> circles;
			std::vector<SolidCircle> solidCircles;
			std::vector<Segment> segments;
			std::vector<Transform> transforms;
			std::vector<Point> points;
		};
		PhysicsDraw m_draw;
	};

}

#endif // KGB_PHYSICS_H
