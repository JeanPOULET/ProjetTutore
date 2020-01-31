#include "Physics.h"

namespace KGB{
    KGB::b2dContactListener m_contactListener;
    namespace{
        constexpr float PHYSICSCALE = 0.02f;
        static constexpr gf::Vector2u cinquante(200, 200);

        b2Vec2 fromVec(gf::Vector2f vec) {
            return { vec.x * PHYSICSCALE, vec.y * PHYSICSCALE };
        }

        gf::Vector2f toVec(b2Vec2 vec) {
            return { vec.x / PHYSICSCALE, vec.y / PHYSICSCALE };
        }
        gf::Color4f toColor(b2Color color) {
            return { color.r, color.g, color.b, color.a * 0.8f };
        }

    }

    enum _entityCategory{
        BABY = 0x0001,
        ENEMY = 0x0004,
        OTHER = 0x0010,
        HARVESTABLE = 0x0014

    };

    void Physics::update() {

        m_body->SetTransform(fromVec(m_baby.getPosition()), 0.0f);
        m_vilainBody1->SetTransform(fromVec(m_vilain1.getPosition()), 0.0f);
        m_vilainBody2->SetTransform(fromVec(m_vilain2.getPosition()), 0.0f);
        m_vilainBody3->SetTransform(fromVec(m_vilain3.getPosition()), 0.0f);
        m_vilainBody4->SetTransform(fromVec(m_vilain4.getPosition()), 0.0f);
        m_vilainBody5->SetTransform(fromVec(m_vilain5.getPosition()), 0.0f);


        m_body->SetLinearVelocity(fromVec(m_baby.getVelocity()));
        m_vilainBody1->SetLinearVelocity(fromVec(m_vilain1.getVelocity()));
        m_vilainBody2->SetLinearVelocity(fromVec(m_vilain2.getVelocity()));
        m_vilainBody3->SetLinearVelocity(fromVec(m_vilain3.getVelocity()));
        m_vilainBody4->SetLinearVelocity(fromVec(m_vilain4.getVelocity()));
        m_vilainBody5->SetLinearVelocity(fromVec(m_vilain5.getVelocity()));

        m_world.Step(1/80.0, 8, 3);

        m_vilain1.setPosition(toVec(m_vilainBody1->GetPosition()));
        m_vilain2.setPosition(toVec(m_vilainBody2->GetPosition()));
        m_vilain3.setPosition(toVec(m_vilainBody3->GetPosition()));
        m_vilain4.setPosition(toVec(m_vilainBody4->GetPosition()));
        m_vilain5.setPosition(toVec(m_vilainBody5->GetPosition()));
        m_baby.setPosition(toVec(m_body->GetPosition()));


    }

     /*

                    TENTATIVE POUR LES MURS -> MAP

     */   

    class PhysicsMaker : public gf::TmxVisitor {

        public:
            PhysicsMaker(b2World& world)
            : m_world(world)
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
                            bodyDef.position = fromVec(position);
                            auto body = m_world.CreateBody(&bodyDef);

                            b2PolygonShape shape;
                            shape.SetAsBox(16.0f * PHYSICSCALE,16.0f*PHYSICSCALE);

                            b2FixtureDef fixtureDef;
                            fixtureDef.density = 1.0f;
                            fixtureDef.friction = 0.0f;
                            fixtureDef.restitution = 0.0f;
                            fixtureDef.shape = &shape;
                            fixtureDef.filter.categoryBits = OTHER;

                            body->CreateFixture(&fixtureDef);
                            
                        }
                        k++;
                    }
                }
            }

        virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
            gf::unused(map);

            gf::Log::info("Parsing object layer '%s'\n", layer.name.c_str());

            for (auto& object : layer.objects) {
                if (object->kind != gf::TmxObject::Tile) {
                    continue;
                }

                auto tile = static_cast<gf::TmxTileObject *>(object.get());

                gf::Vector2f position = tile->position;

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position = fromVec(position);
                
                auto body = m_world.CreateBody(&bodyDef);

                b2CircleShape shape;
                shape.m_radius = 10.0f * PHYSICSCALE;

                b2FixtureDef fixtureDef;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &shape;
                fixtureDef.filter.categoryBits = OTHER;

                if(layer.name =="Objets"){
                    fixtureDef.filter.categoryBits = HARVESTABLE;
                    //body->SetUserData()
                }

                body->CreateFixture(&fixtureDef);
            }
        }

    private:
      b2World& m_world;
    };


    Physics::Physics(const gf::TmxLayers& layers,BabyHero& baby, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4, Enemy& policier5)
        : m_world({ 0.0f, 0.0f })
        , m_body(nullptr)
        , m_baby(baby)
        , m_vilain1(policier1)
        , m_vilainBody1(nullptr)
        , m_vilain2(policier2)
        , m_vilainBody2(nullptr)
        , m_vilain3(policier3)
        , m_vilainBody3(nullptr)
        , m_vilain4(policier4)
        , m_vilainBody4(nullptr)
        , m_vilain5(policier5)
        , m_vilainBody5(nullptr)
        {
        
        //MAP
        PhysicsMaker maker(m_world);
        layers.visitLayers(maker);
        
        
        m_world.SetContactListener(&m_contactListener);
        
        //BABY
        gf::Vector2f initialPosition = m_baby.getPosition();
        
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = fromVec(initialPosition);
        m_body = m_world.CreateBody(&bodyDef);
        m_body->SetUserData(&baby);
        
        //ENEMY 1
        gf::Vector2f initialVilainPosition1 = m_vilain1.getPosition();

        b2BodyDef bodyDefV1;
        bodyDefV1.type = b2_dynamicBody;
        bodyDefV1.position = fromVec(initialVilainPosition1);
        m_vilainBody1 = m_world.CreateBody(&bodyDefV1);
        m_vilainBody1->SetUserData(&policier1);
        //ENEMY 2

        gf::Vector2f initialVilainPosition2 = m_vilain2.getPosition();

        b2BodyDef bodyDefV2;
        bodyDefV2.type = b2_dynamicBody;
        bodyDefV2.position = fromVec(initialVilainPosition2);
        m_vilainBody2 = m_world.CreateBody(&bodyDefV2);
        m_vilainBody2->SetUserData(&policier2);
        //ENEMY 3
        
        gf::Vector2f initialVilainPosition3 = m_vilain3.getPosition();

        b2BodyDef bodyDefV3;
        bodyDefV3.type = b2_dynamicBody;
        bodyDefV3.position = fromVec(initialVilainPosition3);
        m_vilainBody3 = m_world.CreateBody(&bodyDefV3);
        m_vilainBody3->SetUserData(&policier3);
        //ENEMY 4
        
        gf::Vector2f initialVilainPosition4 = m_vilain4.getPosition();

        b2BodyDef bodyDefV4;
        bodyDefV4.type = b2_dynamicBody;
        bodyDefV4.position = fromVec(initialVilainPosition4);
        m_vilainBody4 = m_world.CreateBody(&bodyDefV4);
        m_vilainBody4->SetUserData(&policier4);

        //ENEMY 5
        
        gf::Vector2f initialVilainPosition5 = m_vilain5.getPosition();

        b2BodyDef bodyDefV5;
        bodyDefV5.type = b2_dynamicBody;
        bodyDefV5.position = fromVec(initialVilainPosition5);
        m_vilainBody5 = m_world.CreateBody(&bodyDefV5);

        //Pour le cone 
        
        b2FixtureDef fixtureCone;
        b2PolygonShape shapeCone;
        b2Vec2 vertices[3];
        vertices[0].Set(0,0);
        vertices[1].Set(-20* PHYSICSCALE, 200* PHYSICSCALE);
        vertices[2].Set( 20* PHYSICSCALE, 200* PHYSICSCALE);

        shapeCone.Set(vertices, 3);

        fixtureCone.density = 1.0f;
        fixtureCone.friction = 0.0f;
        fixtureCone.restitution = 0.0f;
        fixtureCone.isSensor = true;
        fixtureCone.filter.categoryBits = ENEMY;
        fixtureCone.filter.maskBits = BABY;
        fixtureCone.shape = &shapeCone;

        m_vilainBody1->CreateFixture(&fixtureCone);
        m_vilainBody2->CreateFixture(&fixtureCone);
        m_vilainBody3->CreateFixture(&fixtureCone);
        m_vilainBody4->CreateFixture(&fixtureCone);

        //Shape & Fixture
        b2PolygonShape shapeVilain;
        shapeVilain.SetAsBox(15.0f*PHYSICSCALE, 18.0f*PHYSICSCALE);

        b2PolygonShape shapeBaby;
        shapeBaby.SetAsBox(13.0f*PHYSICSCALE, 12.0f*PHYSICSCALE);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = ENEMY;
        fixtureDef.shape = &shapeVilain;

        m_vilainBody1->CreateFixture(&fixtureDef);
        m_vilainBody2->CreateFixture(&fixtureDef);
        m_vilainBody3->CreateFixture(&fixtureDef);
        m_vilainBody4->CreateFixture(&fixtureDef);
        m_vilainBody5->CreateFixture(&fixtureDef);

        fixtureDef.shape = &shapeBaby;
        fixtureDef.filter.categoryBits = BABY;
        m_body->CreateFixture(&fixtureDef);
        
        

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
            polygon.addPoint(toVec(vertices[i]));
        }

        polygons.push_back({ std::move(polygon), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        gf::Polygon polygon;

        for (int32 i = 0; i < vertexCount; ++i) {
            polygon.addPoint(toVec(vertices[i]));
        }

        solidPolygons.push_back({ std::move(polygon), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
        circles.push_back({ gf::CircF(toVec(center), radius / PHYSICSCALE), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
        solidCircles.push_back({ gf::CircF(toVec(center), radius / PHYSICSCALE), toVec(axis), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
        segments.push_back({ toVec(p1), toVec(p2), toColor(color) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawTransform(const b2Transform& xf) {
        transforms.push_back({ toVec(xf.p), toVec(xf.q.GetXAxis()), toVec(xf.q.GetYAxis()) });
    }

    void PhysicsDebugger::PhysicsDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
        points.push_back({ toVec(p), size, toColor(color) });
    }
}