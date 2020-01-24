#ifndef KGB_DEBUG_H
#define KGB_DEBUG_H

#include <Box2D/Box2D.h>

#include <vector>

#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Polygon.h>
#include <gf/Vector.h>

namespace KGB{

    class Debug : public gf::Entity {
    public:
      Debug()
      : gf::Entity(10000)
      {

      }


      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:

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

      struct DebugDraw : public b2Draw {
      public:
        DebugDraw();

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

    };
}
#endif