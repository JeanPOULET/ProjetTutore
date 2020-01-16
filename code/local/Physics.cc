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

    Physics::Physics(BabyHero& baby, Enemy& policier)
        : m_world({ 0.0f, 0.0f })
        , m_body(nullptr)
        , m_baby(baby)
        , m_vilain(policier)
        , m_vilainBody(nullptr)
        {

        //BABY
        gf::Vector2f initialPosition = m_baby.getPosition();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = fromVec(initialPosition);
        m_body = m_world.CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(25.0f*PHYSICSCALE, 25.0f*PHYSICSCALE);

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.shape = &shape;

        m_body->CreateFixture(&fixtureDef);


        //ENEMY
        gf::Vector2f initialVilainPosition = m_vilain.getPosition();

        b2BodyDef bodyDefV;
        bodyDefV.type = b2_dynamicBody;
        bodyDefV.position = fromVec(initialVilainPosition);
        m_vilainBody = m_world.CreateBody(&bodyDefV);

        b2PolygonShape shapeV;
        shapeV.SetAsBox(25.0f*PHYSICSCALE, 25.0f*PHYSICSCALE);

        b2FixtureDef fixtureDefV;
        fixtureDefV.density = 1.0f;
        fixtureDefV.friction = 0.0f;
        fixtureDefV.restitution = 0.0f;
        fixtureDefV.shape = &shapeV;

        m_vilainBody->CreateFixture(&fixtureDefV);

    }

    void Physics::update() {
        m_body->SetTransform(fromVec(m_baby.getPosition()), 0.0f);
        m_vilainBody->SetTransform(fromVec(m_vilain.getPosition()), 0.0f);

        m_body->SetLinearVelocity(fromVec(m_baby.getVelocity()));

        m_world.Step(1/80.0, 8, 3);

        m_baby.setPosition(toVec(m_body->GetPosition()));
    }

}