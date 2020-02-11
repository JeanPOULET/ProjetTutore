#include "Object.h"

namespace KGB{
	
	Object::Object(ObjectType type, gf::Vector2f pos, gf::Sprite sprite)
        : gf::Entity(10)
        , m_objectType(type)
        , m_position(pos)
        , m_sprite(sprite)
        , m_body(nullptr)
        {

			this->setAlive();
        }

	void Object::render(gf::RenderTarget& target){
		
		if(this->alive){
			m_sprite.setPosition(m_position);
			m_sprite.setAnchor(gf::Anchor::Center);
			target.draw(m_sprite);
		}
		
    }
    

    void Object::update(gf::Time time){

    }

	void Object::setObjectBody(b2World& world, gf::Vector2f position){
		float physicScale = Physics::getPhysicScale();
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position = Physics::fromVec(m_position);
		
		auto body = world.CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 0.0f;

		if(m_objectType ==ObjectType::CLEF ){
			b2CircleShape shape;
			shape.m_radius = 10.0f * Physics::getPhysicScale();
			fixtureDef.shape = &shape;
			fixtureDef.filter.categoryBits = m_objectType;
			body->CreateFixture(&fixtureDef);
		}else{

			b2PolygonShape shape;
            shape.SetAsBox(16.0f * physicScale,16.0f*physicScale);
			fixtureDef.shape = &shape;
			fixtureDef.filter.categoryBits = m_objectType;
			body->CreateFixture(&fixtureDef);
		}
		
		body->SetUserData((void*) static_cast<KGB::KEntity*>(this));
		
		m_body = body;

	}

	void Object::startContact(int contactwith) {
		
		switch (contactwith){
			case  DataType::Main_Type::BABY:
	
				switch(m_objectType){
				
					case  ObjectType::CLEF :
						gf::Log::debug("LA CLEF\n");
						Clef message;
						//message.position = m_position;
						gMessageManager().sendMessage(&message);
						alive=false;
										
					break;


					default: 
						return;
					break;

				}

			default :

			break;
					
		}
	
    }
}