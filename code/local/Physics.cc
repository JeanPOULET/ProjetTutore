#include "Physics.h"

namespace KGB{
    KGB::b2dContactListener m_contactListener;
    namespace{
        constexpr float PHYSICSCALE = 0.02f;
        static constexpr gf::Vector2u cinquante(200, 200);

        gf::Color4f toColor(b2Color color) {
            return { color.r, color.g, color.b, color.a * 0.8f };
        }

    }
	
	b2Vec2 Physics::fromVec(gf::Vector2f vec) {
            return { vec.x * PHYSICSCALE, vec.y * PHYSICSCALE };
        }

    gf::Vector2f Physics::toVec(b2Vec2 vec) {
            return { vec.x / PHYSICSCALE, vec.y / PHYSICSCALE };
     }

	float Physics::getPhysicScale(){
		
		return PHYSICSCALE;
		
	}

    void Physics::update() {

       	m_baby.updatePhysics_set();
		m_vilain1.updatePhysics_set();
		m_vilain2.updatePhysics_set();
		m_vilain3.updatePhysics_set();
		m_vilain4.updatePhysics_set();
		m_vilain5.updatePhysics_set();

		m_bonus1.updatePhysics_set();
		m_bonus2.updatePhysics_set();
		m_bonus3.updatePhysics_set();

        m_world.Step(1/80.0, 8, 3);

        m_baby.updatePhysics_correction();
        m_vilain1.updatePhysics_correction();
  		m_vilain2.updatePhysics_correction();
		m_vilain3.updatePhysics_correction();
		m_vilain4.updatePhysics_correction();
		m_vilain5.updatePhysics_correction();
        
		m_bonus1.updatePhysics_correction();
		m_bonus2.updatePhysics_correction();
		m_bonus3.updatePhysics_correction();


    }

     /*

                    TENTATIVE POUR LES MURS -> MAP

     */   

    class PhysicsMaker : public gf::TmxVisitor {

        public:
            PhysicsMaker(b2World& world, Objects& objs)
            : m_world(world)
            , m_objs(objs)
            {

            }

            virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
                if (!layer.visible) {
                    return;
                }

                assert(map.orientation == gf::TmxOrientation::Orthogonal);
                gf::Log::info("Parsing layer '%s'\n", layer.name.c_str());

                gf::Vector2i tileSize = map.tileSize;
                gf::Vector2i tilesetTileSize = tileSize;

                int k = 0;
                for (auto& cell : layer.cells) {
                    auto tile = cell;
                    auto gid = cell.gid;
                
                    if (layer.name == "Murs"|| layer.name == "Déco") {
                        if(gid!=0){
                            int i = k % map.mapSize.width;
                            int j = k / map.mapSize.width;
                            //gf::Log::info("i=%d\tj=%d\n",(i*32),j);
                            gf::Vector2f position = gf::Vector2f((i*32)+16,(j*32)+16);

                            b2BodyDef bodyDef;
                            bodyDef.type = b2_staticBody;
                            bodyDef.position = Physics::fromVec(position);
                            auto body = m_world.CreateBody(&bodyDef);

                            b2PolygonShape shape;
                            shape.SetAsBox(16.0f * PHYSICSCALE,16.0f*PHYSICSCALE);

                            b2FixtureDef fixtureDef;
                            fixtureDef.density = 1.0f;
                            fixtureDef.friction = 0.0f;
                            fixtureDef.restitution = 0.0f;
                            fixtureDef.shape = &shape;
                            fixtureDef.filter.categoryBits = DataType::Main_Type::OTHER;

                            body->CreateFixture(&fixtureDef);
                            
                        }
                        k++;
                    }
                }
            }

            virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
                gf::unused(map);

                gf::Log::info("Parsing object layer '%s'\n", layer.name.c_str());

                int numObject =0;
                for (auto& object : layer.objects) {
                    if (object->kind != gf::TmxObject::Tile) {
                        continue;
                    }

                    auto tile = static_cast<gf::TmxTileObject *>(object.get());

                    gf::Vector2f position = tile->position;

                    m_objs.setObjectBody(numObject,m_world,position);
                    numObject++;
                }
            }

        private:
        b2World& m_world;
        Objects& m_objs;
        };


    Physics::Physics(Objects& objs,const gf::TmxLayers& layers,BabyHero& baby, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4, Enemy& policier5, Bonus& bon1, Bonus& bon2, Bonus& bon3)
        : m_world({ 0.0f, 0.0f })
        , m_baby(baby)
        , m_vilain1(policier1)
        , m_vilain2(policier2)
        , m_vilain3(policier3)
        , m_vilain4(policier4)
        , m_vilain5(policier5)
		, m_bonus1(bon1)
		, m_bonus2(bon2)
		, m_bonus3(bon3)
        , m_objs(objs)
        {
        
        //MAP
        PhysicsMaker maker(m_world,m_objs);
        layers.visitLayers(maker);
        
        
        m_world.SetContactListener(&m_contactListener);
        
         //BABY
        
		m_baby.setBodyPhysics(m_world);

        //ENEMIES
			
		m_vilain1.setBodyPhysics(m_world);
		m_vilain2.setBodyPhysics(m_world);
		m_vilain3.setBodyPhysics(m_world);
		m_vilain4.setBodyPhysics(m_world);
		m_vilain5.setBodyPhysics(m_world);
			
		m_bonus1.setBodyPhysics(m_world);
		m_bonus2.setBodyPhysics(m_world);
		m_bonus3.setBodyPhysics(m_world);

    }

    b2World& Physics::getWorld(){
        return m_world;
    }
    namespace {
        constexpr float DebugOutlineThickness = 1.0f;
        constexpr float DebugTransformLength = 0.5f;
    }

    //DEBUG DRAW

    void PhysicsDebugger::setDebug(bool debug) {
        m_state.SetDebugDraw(debug ? &m_draw : nullptr);
    }

    void PhysicsDebugger::render(gf::RenderTarget& target) {
        m_state.DrawDebugData();

        for (auto& polygon : m_draw.polygons) {
            gf::ConvexShape shape(polygon.shape);
            shape.setColor(gf::Color::Transparent);
            shape.setOutlineColor(polygon.color);
            shape.setOutlineThickness(DebugOutlineThickness);
            target.draw(shape);
        }

        for (auto& polygon : m_draw.solidPolygons) {
            gf::ConvexShape shape(polygon.shape);
            shape.setColor(polygon.color);
            target.draw(shape);
        }

        for (auto& circle : m_draw.circles) {
            gf::CircleShape shape(circle.shape);
            shape.setColor(gf::Color::Transparent);
            shape.setOutlineColor(circle.color);
            shape.setOutlineThickness(DebugOutlineThickness);
            target.draw(shape);
        }

        for (auto& circle : m_draw.solidCircles) {
            gf::CircleShape shape(circle.shape);
            shape.setColor(circle.color);
            target.draw(shape);

            gf::Line line(circle.shape.center, circle.shape.center + circle.axis);
            line.setWidth(2.5f * DebugOutlineThickness);
            line.setColor(gf::Color::darker(circle.color, 0.3f));
            target.draw(line);
        }

        for (auto& segment : m_draw.segments) {
            gf::Line curve(segment.p1, segment.p2);
            curve.setWidth(DebugOutlineThickness);
            curve.setColor(segment.color);
            target.draw(curve);
        }

        for (auto& transform : m_draw.transforms) {
            gf::Line lineX(transform.position, transform.position + DebugTransformLength * transform.xAxis);
            lineX.setWidth(DebugOutlineThickness);
            lineX.setColor(gf::Color::Red);
            target.draw(lineX);

            gf::Line lineY(transform.position, transform.position + DebugTransformLength * transform.yAxis);
            lineY.setWidth(DebugOutlineThickness);
            lineY.setColor(gf::Color::Green);
            target.draw(lineY);
        }

        for (auto& point : m_draw.points) {
            gf::CircleShape circle(point.size / 2);
            circle.setColor(point.color);
            circle.setAnchor(gf::Anchor::Center);
            circle.setPosition(point.position);
            target.draw(circle);
        }

        m_draw.polygons.clear();
        m_draw.solidPolygons.clear();
        m_draw.circles.clear();
        m_draw.solidCircles.clear();
        m_draw.segments.clear();
        m_draw.transforms.clear();
        m_draw.points.clear();
    }

    PhysicsDebugger::PhysicsDraw::PhysicsDraw()
    {
        SetFlags(b2Draw::e_shapeBit /* | b2Draw::e_aabbBit */ | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    }

    void PhysicsDebugger::PhysicsDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        gf::Polygon polygon;

        for (int32 i = 0; i < vertexCount; ++i) {
            polygon.addPoint(Physics::toVec(vertices[i]));
        }

        polygons.push_back({ std::move(polygon), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        gf::Polygon polygon;

        for (int32 i = 0; i < vertexCount; ++i) {
            polygon.addPoint(Physics::toVec(vertices[i]));
        }

        solidPolygons.push_back({ std::move(polygon), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
        circles.push_back({ gf::CircF(Physics::toVec(center), radius / PHYSICSCALE), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
        solidCircles.push_back({ gf::CircF(Physics::toVec(center), radius / PHYSICSCALE), Physics::toVec(axis), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
        segments.push_back({ Physics::toVec(p1), Physics::toVec(p2), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawTransform(const b2Transform& xf) {
        transforms.push_back({ Physics::toVec(xf.p), Physics::toVec(xf.q.GetXAxis()), Physics::toVec(xf.q.GetYAxis()) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
        points.push_back({ Physics::toVec(p), size, toColor(color) });
    }
}