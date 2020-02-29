#include "BabyHero.h"
#include "gf/Log.h"

#include <gf/Anchor.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>
#include <gf/Unused.h>
#include <gf/Font.h>

namespace KGB{

	static constexpr float SoundVolume = 100.0f;
	
    BabyHero::BabyHero(gf::Vector2f position)
        : m_status(Status::Waiting)
		, m_body(nullptr)
		, m_WalkingSound(gResourceManager().getSound("sounds/walkingSound.wav"))
		, m_TakingSound(gResourceManager().getSound("sounds/takingSound.wav"))
		, m_SpeedBonusSound(gResourceManager().getSound("sounds/speedBonusSound.wav"))
		, m_InvisibleBonusSound(gResourceManager().getSound("sounds/invisibleBonusSound.wav"))
		, tempoSound(0)
    {
			
        dynamics.m_position = position;
        dynamics.m_velocity = gf::Vector2f(0, 0);
        graphics.m_texture = &gResourceManager().getTexture("Image/KGBaby_animation.png");
        graphics.m_orientation = gf::Orientation::South;
        graphics.m_currentAnimation = nullptr;
        loadAnimation(graphics.m_moveSouth, 0);
        loadAnimation(graphics.m_moveEast, 1);
        loadAnimation(graphics.m_moveWest, 2);
        loadAnimation(graphics.m_moveNorth, 3);
        loadAnimation(graphics.m_waitSouth, 4);
        loadAnimation(graphics.m_waitEast, 5);
        loadAnimation(graphics.m_waitWest, 6);
        loadAnimation(graphics.m_waitNorth, 7);

		m_WalkingSound.setVolume(SoundVolume/2);
		m_TakingSound.setVolume(SoundVolume);
		m_SpeedBonusSound.setVolume(SoundVolume);
		m_InvisibleBonusSound.setVolume(SoundVolume);
    }

	int BabyHero::getInvi(){
		return invi_muni;
	}

	int BabyHero::getSpeed(){
		return speed_muni;
	}

    void BabyHero::setVelocity(gf::Vector2f velocity) {
        dynamics.m_velocity = velocity;  
    }

	gf::Vector2f BabyHero::getVelocity(){
		return dynamics.m_velocity;
	}

    void BabyHero::updateOrientation(int orientation){
		switch(orientation){
			case 0:
				graphics.m_orientation =  gf::Orientation::South;
			break;
			case 1:
				graphics.m_orientation =  gf::Orientation::North;
			break;
			case 2:
				graphics.m_orientation =  gf::Orientation::East;
			break;
			case 3:
				graphics.m_orientation =  gf::Orientation::West;
			break;
			default:
			break;
		}
    }

    void BabyHero::update(gf::Time time) {
        dynamics.m_position += time.asSeconds() * dynamics.m_velocity;
		tempoSound++;
        
        if(dynamics.m_velocity.x == 0 && dynamics.m_velocity.y == 0){
          	m_status = Status::Waiting;
        }else{
          	m_status = Status::Walking;
			if(tempoSound>=30){ //toutes les demi-seconde
				m_WalkingSound.play();
				tempoSound=0;
			}

		}

        if(m_status == Status::Walking){
			switch (graphics.m_orientation) {
				case gf::Orientation::South:
					graphics.m_currentAnimation = &graphics.m_moveSouth;
				break;
				case gf::Orientation::North:
					graphics.m_currentAnimation = &graphics.m_moveNorth;
				break;
				case gf::Orientation::East:
					graphics.m_currentAnimation = &graphics.m_moveEast;
				break;
				case gf::Orientation::West:
					graphics.m_currentAnimation = &graphics.m_moveWest;
				break;
				default:

            	break;
          }
        }else if(m_status == Status::Waiting){
			switch (graphics.m_orientation) {
				case gf::Orientation::South:
					graphics.m_currentAnimation = &graphics.m_waitSouth;
				break;
				case gf::Orientation::North:
					graphics.m_currentAnimation = &graphics.m_waitNorth;
				break;
				case gf::Orientation::East:
					graphics.m_currentAnimation = &graphics.m_waitEast;
				break;
				case gf::Orientation::West:
					graphics.m_currentAnimation = &graphics.m_waitWest;
				break;
				default:

				break;
         	}
        }else if(m_status == Status::Immolate){

        }

        assert(graphics.m_currentAnimation);
        graphics.m_currentAnimation->update(time);
    }

    void BabyHero::render(gf::RenderTarget& target){
        gf::AnimatedSprite animated;
        animated.setAnimation(*graphics.m_currentAnimation);
		if(timeout > 0){
			if(invi_active){
				animated.setColor({1, 1, 1, 0.5});
			}
			if(speed_active){
				animated.setColor({0, 0.6, 1.0, 1});
			}
		}else{
				speed_active = false;
				invi_active = false;
			
		}
		animated.setScale(0.75f);
        animated.setPosition(dynamics.m_position);
        animated.setAnchor(gf::Anchor::Center);		
        target.draw(animated);
        
    }
    

    gf::Vector2f BabyHero::getPosition(){
        return dynamics.m_position;
    }

    void BabyHero::setPosition(gf::Vector2f position){
        dynamics.m_position = position;
    }

    void BabyHero::loadAnimation(gf::Animation &animation, int line) {
        static constexpr gf::Vector2f TextureSize = { 256.0f, 512.0f };
        static constexpr gf::Vector2f FrameSize = { 64.0f, 64.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/5.0f);

        for (int i = 0; i < 4; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(*graphics.m_texture, frame, FrameDuration);
        }
        
    }

    void BabyHero::startContact(int contactwith, int filter) {
		
		switch (contactwith){
				
			case  DataType::Main_Type::ENEMY:
				m_ennemycontact.push_back(0);
			break;

			case  ObjectType::ENTRY:
				if(libre){
					Victory message;
					gMessageManager().sendMessage(&message);
					gf::Log::debug("LA VICTOIRE\n");
				}
			break;

			case  ObjectType::CLEF:
				m_TakingSound.play();
				gf::Log::debug("Je peux sortir\n");
				libre=true;
			break;

			case  DataType::Main_Type::HARVESTABLE :
				m_TakingSound.play();
				switch(filter){
					case DataType::Bonus_Type::STUNNING_DIAPERS:
						++stun_muni;
						gf::Log::debug("Projectile %d:\n", invi_muni);
					break;
					case DataType::Bonus_Type::INVISIBLE_DIAPERS:
						++invi_muni;
						gf::Log::debug("Invisible %d:\n", invi_muni);
					break;
					case DataType::Bonus_Type::SPEED_DIAPERS:
						++speed_muni;
						gf::Log::debug("Projectile %d:\n", invi_muni);
					break;
					default : 
						++stun_muni;
						gf::Log::debug("(Default) Projectile : %d:\n", invi_muni);
					break;
				}
			break;
			default:
			
			break;
				
		}
	
    }
    void BabyHero::endContact(int contactwith, int filter) { 
		
		switch (contactwith){
			
			case  DataType::Main_Type::ENEMY:
				m_ennemycontact.pop_back();
			break;
			
			default:
			break;
				
		}
	
	}

	void BabyHero::setBodyPhysics(b2World& world){

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Physics::fromVec(this->getPosition());
		m_body = world.CreateBody(&bodyDef);

		m_body->SetUserData((void*) static_cast<KGB::KEntity*>(this));

		b2PolygonShape shapeBaby;
		shapeBaby.SetAsBox(13.0f*Physics::getPhysicScale(), 12.0f*Physics::getPhysicScale());

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.filter.categoryBits = DataType::Main_Type::BABY;
		fixtureDef.shape = &shapeBaby;

		m_body->CreateFixture(&fixtureDef);

    }

    void BabyHero::updatePhysics_set(){

		m_body->SetTransform(Physics::fromVec(this->getPosition()), 0.0f);
		m_body->SetLinearVelocity(Physics::fromVec(this->getVelocity()));

    }

    void BabyHero::updatePhysics_correction(){
	
		setPosition(Physics::toVec(m_body->GetPosition()));
		if(timeout > 0){
			--timeout;
		}else{
			invi_active = false;
			speed_active = false;
		}

		if(m_ennemycontact.size() > 0 && !invi_active){
			gf::Log::debug("Un garde m'a repéré!\n");
			GameOver message;
			gMessageManager().sendMessage(&message);
		}

    }
	
	bool BabyHero::getSpeedActive(){
		return speed_active;
	}
	
	int BabyHero::getNbStunning(){
		return stun_muni;
	}
	
	void BabyHero::setStun(){
	
		--stun_muni;
		gf::Log::info("munition stun : %d\n", stun_muni);

	}
	
	void BabyHero::setSpeed(int time){
		
		if(timeout <= 0 && speed_muni > 0){
			m_SpeedBonusSound.play();
			speed_active = true;
			invi_active = false;
			timeout = time;
			--speed_muni;
			gf::Log::info("munition speed : %d\n", speed_muni);
		}
	}

    void BabyHero::setInvisible(int time){
		
		if(timeout <= 0 && invi_muni > 0){
			m_InvisibleBonusSound.play();
			invi_active = true;
			speed_active = false;
			timeout = time;
			--invi_muni;
			gf::Log::info("munition inv : %d\n", invi_muni);
		}
    }
	
}
