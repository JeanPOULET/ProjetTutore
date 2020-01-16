#include "Physics.h"

namespace KGB{

    namespace{
        constexpr float PHYSICSCALE = 0.02f;

        b2Vec2 fromVec(gf::Vector2f vec) {
            return { vec.x * PHYSICSCALE, vec.y * PHYSICSCALE };
        }

        gf::Vector2f toVec(b2Vec2 vec) {
            return { vec.x / PHYSICSCALE, vec.y / PHYSICSCALE };
        }
    }

    Physics::Physics(BabyHero& baby, Enemy& policier1, Enemy& policier2, Enemy& policier3, Enemy& policier4)
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
        b2PolygonShape shape;
        shape.SetAsBox(25.0f*PHYSICSCALE, 25.0f*PHYSICSCALE);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.shape = &shape;

        m_body->CreateFixture(&fixtureDef);

        m_vilainBody1->CreateFixture(&fixtureDef);
        m_vilainBody2->CreateFixture(&fixtureDef);
        m_vilainBody3->CreateFixture(&fixtureDef);
        m_vilainBody4->CreateFixture(&fixtureDef);

    }

    void Physics::update() {
        m_body->SetTransform(fromVec(m_baby.getPosition()), 0.0f);
        m_vilainBody1->SetTransform(fromVec(m_vilain1.getPosition()), 0.0f);
        m_vilainBody2->SetTransform(fromVec(m_vilain2.getPosition()), 0.0f);
        m_vilainBody3->SetTransform(fromVec(m_vilain3.getPosition()), 0.0f);
        m_vilainBody4->SetTransform(fromVec(m_vilain4.getPosition()), 0.0f);

        m_body->SetLinearVelocity(fromVec(m_baby.getVelocity()));

        m_world.Step(1/80.0, 8, 3);

        m_baby.setPosition(toVec(m_body->GetPosition()));
    }

}