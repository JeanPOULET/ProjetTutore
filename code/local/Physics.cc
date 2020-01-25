#include "Physics.h"

namespace KGB{

    namespace{
        constexpr float PHYSICSCALE = 0.02f;
        static constexpr gf::Vector2u cinquante(200, 200);

        b2Vec2 fromVec(gf::Vector2f vec) {
            return { vec.x * PHYSICSCALE, vec.y * PHYSICSCALE };
        }

        gf::Vector2f toVec(b2Vec2 vec) {
            return { vec.x / PHYSICSCALE, vec.y / PHYSICSCALE };
        }

    }



    void Physics::update() {
        m_body->SetTransform(fromVec(m_baby.getPosition()), 0.0f);
        m_vilainBody1->SetTransform(fromVec(m_vilain1.getPosition()), 0.0f);
        m_vilainBody2->SetTransform(fromVec(m_vilain2.getPosition()), 0.0f);
        m_vilainBody3->SetTransform(fromVec(m_vilain3.getPosition()), 0.0f);
        m_vilainBody4->SetTransform(fromVec(m_vilain4.getPosition()), 0.0f);

        m_body->SetLinearVelocity(fromVec(m_baby.getVelocity()));
        m_vilainBody1->SetLinearVelocity(fromVec(m_vilain1.getVelocity()));
        m_vilainBody2->SetLinearVelocity(fromVec(m_vilain2.getVelocity()));
        m_vilainBody3->SetLinearVelocity(fromVec(m_vilain3.getVelocity()));
        m_vilainBody4->SetLinearVelocity(fromVec(m_vilain4.getVelocity()));

        m_world.Step(1/80.0, 8, 3);

        m_vilain1.setPosition(toVec(m_vilainBody1->GetPosition()));
        m_vilain2.setPosition(toVec(m_vilainBody2->GetPosition()));
        m_vilain3.setPosition(toVec(m_vilainBody3->GetPosition()));
        m_vilain4.setPosition(toVec(m_vilainBody4->GetPosition()));
        m_baby.setPosition(toVec(m_body->GetPosition()));
        
        
    }

     /*

                    TENTATIVE POUR LES MURS -> MAP

     */   

    class PhysicsMaker : public gf::TmxVisitor {
        static constexpr int Sol = 0;
        static constexpr int Walls = 1;

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
            gf::Array2D<int, int> biomes(map.mapSize, Sol);

            int k = 0;
            for (auto& cell : layer.cells) {
                auto tile = cell;
                auto gid = cell.gid;
            
                if (layer.name == "Murs"|| layer.name == "Déco" || layer.name == "Objets") {
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

            if (layer.name == "Murs" || layer.name == "Déco" || layer.name == "Objets") {
                gf::Vector2f position = tile->position + gf::Vector2f(384 / 2, -70);

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position = fromVec(position);
                auto body = m_world.CreateBody(&bodyDef);

                b2PolygonShape shape;
                shape.m_radius = 10.0f * PHYSICSCALE;

                b2FixtureDef fixtureDef;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.shape = &shape;

                body->CreateFixture(&fixtureDef);
            }
        }
    }

    private:
      b2World& m_world;
    };

    constexpr int PhysicsMaker::Sol;
    constexpr int PhysicsMaker::Walls;

    Physics::Physics(const gf::TmxLayers& layers,BabyHero& baby, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4)
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
        {
        
        //MAP
        PhysicsMaker maker(m_world);
        layers.visitLayers(maker);

        //BABY
        gf::Vector2f initialPosition = m_baby.getPosition();
        
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = fromVec(initialPosition);
        m_body = m_world.CreateBody(&bodyDef);

        //ENEMY 1
        gf::Vector2f initialVilainPosition1 = m_vilain1.getPosition();

        b2BodyDef bodyDefV1;
        bodyDefV1.type = b2_dynamicBody;
        bodyDefV1.position = fromVec(initialVilainPosition1);
        m_vilainBody1 = m_world.CreateBody(&bodyDefV1);

        //ENEMY 2

        gf::Vector2f initialVilainPosition2 = m_vilain2.getPosition();

        b2BodyDef bodyDefV2;
        bodyDefV2.type = b2_dynamicBody;
        bodyDefV2.position = fromVec(initialVilainPosition2);
        m_vilainBody2 = m_world.CreateBody(&bodyDefV2);

        //ENEMY 3
        
        gf::Vector2f initialVilainPosition3 = m_vilain3.getPosition();

        b2BodyDef bodyDefV3;
        bodyDefV3.type = b2_dynamicBody;
        bodyDefV3.position = fromVec(initialVilainPosition3);
        m_vilainBody3 = m_world.CreateBody(&bodyDefV3);

        //ENEMY 4
        
        gf::Vector2f initialVilainPosition4 = m_vilain4.getPosition();

        b2BodyDef bodyDefV4;
        bodyDefV4.type = b2_dynamicBody;
        bodyDefV4.position = fromVec(initialVilainPosition4);
        m_vilainBody4 = m_world.CreateBody(&bodyDefV4);


        //Shape & Fixture
        b2PolygonShape shapeVilain;
        shapeVilain.SetAsBox(15.0f*PHYSICSCALE, 20.0f*PHYSICSCALE);

        b2PolygonShape shapeBaby;
        shapeBaby.SetAsBox(13.0f*PHYSICSCALE, 12.0f*PHYSICSCALE);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.shape = &shapeVilain;

        m_vilainBody1->CreateFixture(&fixtureDef);
        m_vilainBody2->CreateFixture(&fixtureDef);
        m_vilainBody3->CreateFixture(&fixtureDef);
        m_vilainBody4->CreateFixture(&fixtureDef);

        fixtureDef.shape = &shapeBaby;
        m_body->CreateFixture(&fixtureDef);

    }
}