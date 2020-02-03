#include "Bonus.h"

namespace KGB{
  
    Bonus::Bonus(gf::Vector2f position, DataType::Bonus_Type type)
        : m_body(nullptr)
		, m_status(Status::Waiting)
    {
	
        dynamics.m_position = position;
        dynamics.m_velocity = gf::Vector2f(0,0);
        graphics.m_orientation = gf::Orientation::South;

        switch (type){

            case DataType::Bonus_Type::STUNNING_DIAPERS :
                graphics.m_texture = &gResourceManager().getTexture("Image/Couche_animation.png");
            break;
            
            default :
            
            break;
        }

        graphics.m_currentAnimation = &graphics.m_waitSouth;	
        loadAnimation(graphics.m_waitSouth, 0);
			
    }


    void Bonus::setVelocity(gf::Vector2f velocity) {

	dynamics.m_velocity = velocity; 
 
    }

	gf::Vector2f Bonus::getVelocity(){
		return dynamics.m_velocity;
	}

    void Bonus::setBodyPhysics(b2World& world){

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = Physics::fromVec(this->getPosition());
        m_body = world.CreateBody(&bodyDef);

        m_body->SetUserData((void*) static_cast<KGB::KEntity*>(this));

        b2PolygonShape shapeBonus;
        shapeBonus.SetAsBox(13.0f*Physics::getPhysicScale(), 12.0f*Physics::getPhysicScale());
        
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = DataType::Main_Type::HARVESTABLE;
        fixtureDef.shape = &shapeBonus;

        m_body->CreateFixture(&fixtureDef);

    }

    void Bonus::updatePhysics_set(){

        m_body->SetTransform(Physics::fromVec(this->getPosition()), 0.0f);
        m_body->SetLinearVelocity(Physics::fromVec(this->getVelocity()));

    }

    void Bonus::updatePhysics_correction(){

	    setPosition(Physics::toVec(m_body->GetPosition()));

    }

    void Bonus::update(gf::Time time) {
        dynamics.m_position += time.asSeconds() * dynamics.m_velocity;
        
        if(dynamics.m_velocity.x == 0 && dynamics.m_velocity.y == 0){
          	m_status = Status::Waiting;
        }else{
          	m_status = Status::Moving;
        }		
		
        assert(graphics.m_currentAnimation);
        graphics.m_currentAnimation->update(time);
    }

    void Bonus::render(gf::RenderTarget& target){
        gf::AnimatedSprite animated;
        animated.setAnimation(*graphics.m_currentAnimation);
        animated.setScale(0.4f);
        animated.setPosition(dynamics.m_position);
        animated.setAnchor(gf::Anchor::Center);
        target.draw(animated);
    }
    

    gf::Vector2f Bonus::getPosition(){
        return dynamics.m_position;
    }

    void Bonus::setPosition(gf::Vector2f position){
        dynamics.m_position = position;
    }

    void Bonus::loadAnimation(gf::Animation &animation, int line) {
        static constexpr gf::Vector2f TextureSize = { 512.0f, 64.0f };
        static constexpr gf::Vector2f FrameSize = { 64.0f, 64.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/6.0f);

        for (int i = 0; i < 8; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(*graphics.m_texture, frame, FrameDuration);
        }
        
    }
	
	void Bonus::startContact(int contactwith) {
		
		switch (contactwith){
				
			case  DataType::Main_Type::BABY:		gf::Log::debug("MORE DIAPERS\n");
													break;
			default: 								break;
				
		}
	
    }
    void Bonus::endContact(int contactwith) { 
		
		switch (contactwith){
			
			default: 	gf::Log::debug("OH NO");
						break;
				
		}
	
	}

}
