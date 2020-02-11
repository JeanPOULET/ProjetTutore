#include "Entry.h"

namespace KGB{
    
    Entry::Entry(gf::Vector2f position)
        : gf::Entity(10)
        , m_position(position)
        , m_body(nullptr)
    {
        gf::Log::debug("Noon\n");
        
        
    }

    void Entry::startContact(int contactwith){
        switch(contactwith){
            case  DataType::Main_Type::BABY :
                gf::Log::debug("Oui");
            break;

            default :
                gf::Log::debug("Non");
            break;
        }
        

    }

    void Entry::update(gf::Time time){

    }

    
    void Entry::setObjectBody(b2World& world, gf::Vector2f position){

        gf::Log::debug("LE body\n");
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = Physics::fromVec(position);
        auto body = world.CreateBody(&bodyDef);
        body->SetUserData((void*) static_cast<KGB::KEntity*>(this));
        b2PolygonShape shape;
        shape.SetAsBox(16.0f * Physics::getPhysicScale(),16.0f*Physics::getPhysicScale());

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.shape = &shape;
        fixtureDef.filter.categoryBits = DataType::Main_Type::ENTRY;
        
        body->CreateFixture(&fixtureDef);
		
		m_body = body;
    }


}