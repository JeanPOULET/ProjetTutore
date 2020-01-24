#include "Debug.h"
#include <Box2D/Common/b2Draw.h>

namespace KGB{
	namespace {
		constexpr float PhysicsScale = 0.02f;

		gf::Vector2f toVec(b2Vec2 vec) {
		return { vec.x / PhysicsScale, vec.y / PhysicsScale };
		}

		gf::Color4f toColor(b2Color color) {
		return { color.r, color.g, color.b, color.a * 0.8f };
		}

	}
	Debug::Debug()
	{
		SetFlags(b2Draw::e_shapeBit /* | b2Draw::e_aabbBit */ | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
	}

	void Debug::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		gf::Polygon polygon;

		for (int32 i = 0; i < vertexCount; ++i) {
			polygon.addPoint(toVec(vertices[i]));
		}

		polygons.push_back({ std::move(polygon), toColor(color) });
	}

	void Debug::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		gf::Polygon polygon;

		for (int32 i = 0; i < vertexCount; ++i) {
			polygon.addPoint(toVec(vertices[i]));
		}

		solidPolygons.push_back({ std::move(polygon), toColor(color) });
	}

	void Debug::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
		circles.push_back({ gf::CircF(toVec(center), radius / PhysicsScale), toColor(color) });
	}

	void Debug::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
		solidCircles.push_back({ gf::CircF(toVec(center), radius / PhysicsScale), toVec(axis), toColor(color) });
	}

	void Debug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		segments.push_back({ toVec(p1), toVec(p2), toColor(color) });
	}

	void Debug::DrawTransform(const b2Transform& xf) {
		transforms.push_back({ toVec(xf.p), toVec(xf.q.GetXAxis()), toVec(xf.q.GetYAxis()) });
	}

	void Debug::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
		points.push_back({ toVec(p), size, toColor(color) });
	}

}
