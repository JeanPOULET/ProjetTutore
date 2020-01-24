#include "Enemy.h"


namespace KGB{
  
	Enemy::Enemy(gf::Vector2f position, PathType path, gf::Orientation ori)
        : m_spawn(position)
		, m_path(path)
		, m_status(Status::Walking)
        {
			
		dynamics.m_position = position;
        dynamics.m_velocity = gf::Vector2f(0, 0);
        graphics.m_texture = &gResourceManager().getTexture("Image/Polizei_animation.png");
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
    }

	gf::Vector2f Enemy::getVelocity(){
		return dynamics.m_velocity;
	}
	
    void Enemy::setVelocity(gf::Vector2f velocity) {
        dynamics.m_velocity = velocity;
    }

	void Enemy::round(){
		if(dynamics.m_position.x <= m_spawn.x && dynamics.m_position.y >= m_spawn.y + 200.0f){
			graphics.m_orientation = gf::Orientation::East;
			dynamics.m_velocity = gf::Vector2f(50.0,0);
		}else if(dynamics.m_position.x >= m_spawn.x + 200.0f && dynamics.m_position.y >= m_spawn.y + 200.0f){
			graphics.m_orientation = gf::Orientation::North;
			dynamics.m_velocity = gf::Vector2f(0,-50.0);
		}else if(dynamics.m_position.x >= m_spawn.x + 200.0f && dynamics.m_position.y <= m_spawn.y){
			graphics.m_orientation = gf::Orientation::West;
			dynamics.m_velocity = gf::Vector2f(-50.0,0);
		}else if(dynamics.m_position.x <= m_spawn.x && dynamics.m_position.y <= m_spawn.y){
			graphics.m_orientation = gf::Orientation::South;
			dynamics.m_velocity = gf::Vector2f(0,50.0);
		}
	}

	void Enemy::lineV(){
		if(dynamics.m_position.x <= m_spawn.x){
			graphics.m_orientation = gf::Orientation::East;
			dynamics.m_velocity = gf::Vector2f(50.0,0);
		}else if(dynamics.m_position.x >= m_spawn.x + 200.0f){
			graphics.m_orientation = gf::Orientation::West;
			dynamics.m_velocity = gf::Vector2f(-50.0,0);
		}
	}

	void Enemy::lineH(){
		if(dynamics.m_position.y >= m_spawn.y + 200.0f){
			graphics.m_orientation = gf::Orientation::North;
			dynamics.m_velocity = gf::Vector2f(0,-50.0);
		}else if(dynamics.m_position.y <= m_spawn.y){
			graphics.m_orientation = gf::Orientation::South;
			dynamics.m_velocity = gf::Vector2f(0,50.0);
		}
	}

    void Enemy::update(gf::Time time) {
        
		dynamics.m_position += time.asSeconds() * dynamics.m_velocity;
		
        if(m_path == PathType::Round){
			round();
		}else if(m_path == PathType::VerticalLine){
			lineV();
		}else if(m_path == PathType::HorizontalLine){
			lineH();
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
				// assert(false);
				break;
			}
		}else{
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
				// assert(false);
				break;
         	}
		}
        
        assert(graphics.m_currentAnimation);
        graphics.m_currentAnimation->update(time);
    }

    void Enemy::render(gf::RenderTarget& target){
        gf::AnimatedSprite animated;
        assert(graphics.m_currentAnimation);
        animated.setAnimation(*graphics.m_currentAnimation);
		animated.setPosition(dynamics.m_position);
        animated.setScale(0.75f);
        animated.setAnchor(gf::Anchor::Center);
        target.draw(animated);
    }
    

    gf::Vector2f Enemy::getPosition(){
        return dynamics.m_position;
    }

    void Enemy::setPosition(gf::Vector2f position){
        dynamics.m_position = position;
    }

    void Enemy::loadAnimation(gf::Animation &animation, int line) {
        static constexpr gf::Vector2f TextureSize = { 256.0f, 768.0f };
        static constexpr gf::Vector2f FrameSize = { 64.0f, 100.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/5.0f);

        for (int i = 0; i < 4; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(*graphics.m_texture, frame, FrameDuration);
        }
    }
}
