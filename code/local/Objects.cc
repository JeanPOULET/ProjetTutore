#include "Objects.h"

namespace KGB{
    Objects::Objects()
        : gf::Entity(10)
        {

        }


	void Objects::render(gf::RenderTarget& target){
		/*static constexpr gf::Vector2f AssetSize = { 32.0f, 32.0f };
		static constexpr gf::Vector2f TilesetSize = { 256.0f, 4256.0f };*/
    	for (auto &obj: m_objects) {
			//gf::RectF textureRect = gf::RectF::fromPositionSize({ static_cast<int>(obj.m_objectType) * AssetSize.width / TilesetSize.width, 0.0f }, AssetSize / TilesetSize);
			obj.m_sprite.setPosition(obj.m_position);
			obj.m_sprite.setAnchor(gf::Anchor::Center);
			target.draw(obj.m_sprite);
		}
    }
    

    void Objects::update(gf::Time time){

    }

	void Objects::addObject(ObjectType type, gf::Vector2f position, gf::Sprite sprite){

		m_objects.push_back({type,position,sprite});
	}

	void Objects::setObjectBody(int num, b2World& world, gf::Vector2f position){
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position = Physics::fromVec(position);
		
		auto body = world.CreateBody(&bodyDef);
		
		b2CircleShape shape;
		shape.m_radius = 10.0f * Physics::getPhysicScale();

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = ObjectType::CLEF;
		body->CreateFixture(&fixtureDef);
		m_objects[num].m_body = body;
		body->SetUserData((void*) static_cast<KGB::KEntity*>(&m_objects[num]));

	}

	/*int Objects::getEntityType(){
		return ObjectType::CLEF;
	}*/

	void object::startContact(int contactwith) {
		
		switch (contactwith){
			
			case  DataType::Main_Type::BABY:
				gf::Log::debug("LA CLEF\n");
				Victory message;
				//message.position = m_position;
				gMessageManager().sendMessage(&message);
				
				//delete this;
				
			break;

			default: 
				return;
			break;
				
		}
	
    }
}