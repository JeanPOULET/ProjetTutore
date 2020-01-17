#include "Enemy.h"


namespace KGB{
  
    Enemy::Enemy(gf::Vector2f position)
        : m_position(position)
		, m_spawn(position)
        , m_velocity(0, 0)
        , m_texture(gResourceManager().getTexture("Image/Polizei_animation.png"))
        , m_orientation(gf::Orientation::South)
        , m_currentAnimation(nullptr)
		, m_path(PathType::Static)
		, m_status(Status::Waiting)
        {
        loadAnimation(m_moveSouth, 0);
        loadAnimation(m_moveEast, 1);
        loadAnimation(m_moveWest, 2);
        loadAnimation(m_moveNorth, 3);
		loadAnimation(m_waitSouth, 4);
        loadAnimation(m_waitEast, 5);
        loadAnimation(m_waitWest, 6);
        loadAnimation(m_waitNorth, 7);
    }

	Enemy::Enemy(gf::Vector2f position, PathType path, gf::Orientation ori)
        : m_position(position)
		, m_spawn(position)
        , m_velocity(0, 50.0)
        , m_texture(gResourceManager().getTexture("Image/Polizei_animation.png"))
        , m_orientation(ori)
        , m_currentAnimation(nullptr)
		, m_path(path)
		, m_status(Status::Walking)
        {
        loadAnimation(m_moveSouth, 0);
        loadAnimation(m_moveEast, 1);
        loadAnimation(m_moveWest, 2);
        loadAnimation(m_moveNorth, 3);
		loadAnimation(m_waitSouth, 4);
        loadAnimation(m_waitEast, 5);
        loadAnimation(m_waitWest, 6);
        loadAnimation(m_waitNorth, 7);
    }

	gf::Vector2f Enemy::getVelocity(){
		return m_velocity;
	}
	
    void Enemy::setVelocity(gf::Vector2f velocity) {
        m_velocity = velocity;
    }

	void Enemy::round(){
		if(m_position.x <= m_spawn.x && m_position.y >= m_spawn.y + 200.0f){
			m_orientation = gf::Orientation::East;
			m_velocity = gf::Vector2f(50.0,0);
		}else if(m_position.x >= m_spawn.x + 200.0f && m_position.y >= m_spawn.y + 200.0f){
			m_orientation = gf::Orientation::North;
			m_velocity = gf::Vector2f(0,-50.0);
		}else if(m_position.x >= m_spawn.x + 200.0f && m_position.y <= m_spawn.y){
			m_orientation = gf::Orientation::West;
			m_velocity = gf::Vector2f(-50.0,0);
		}else if(m_position.x <= m_spawn.x && m_position.y <= m_spawn.y){
			m_orientation = gf::Orientation::South;
			m_velocity = gf::Vector2f(0,50.0);
		}
	}

	void Enemy::lineV(){
		if(m_position.x <= m_spawn.x){
			m_orientation = gf::Orientation::East;
			m_velocity = gf::Vector2f(50.0,0);
		}else if(m_position.x >= m_spawn.x + 200.0f){
			m_orientation = gf::Orientation::West;
			m_velocity = gf::Vector2f(-50.0,0);
		}
	}

	void Enemy::lineH(){
		if(m_position.y >= m_spawn.y + 200.0f){
			m_orientation = gf::Orientation::North;
			m_velocity = gf::Vector2f(0,-50.0);
		}else if(m_position.y <= m_spawn.y){
			m_orientation = gf::Orientation::South;
			m_velocity = gf::Vector2f(0,50.0);
		}
	}

    void Enemy::update(gf::Time time) {
        
		m_position += time.asSeconds() * m_velocity;
		
        if(m_path == PathType::Round){
			round();
		}else if(m_path == PathType::VerticalLine){
			lineV();
		}else if(m_path == PathType::HorizontalLine){
			lineH();
		}
		if(m_status == Status::Walking){
			switch (m_orientation) {
				case gf::Orientation::South:
					m_currentAnimation = &m_moveSouth;
					break;
				case gf::Orientation::North:
					m_currentAnimation = &m_moveNorth;
					break;
				case gf::Orientation::East:
					m_currentAnimation = &m_moveEast;
					break;
				case gf::Orientation::West:
					m_currentAnimation = &m_moveWest;
					break;
				default:
				// assert(false);
				break;
			}
		}else{
			switch (m_orientation) {
				case gf::Orientation::South:
					m_currentAnimation = &m_waitSouth;
				break;
				case gf::Orientation::North:
					m_currentAnimation = &m_waitNorth;
				break;
				case gf::Orientation::East:
					m_currentAnimation = &m_waitEast;
				break;
				case gf::Orientation::West:
					m_currentAnimation = &m_waitWest;
				break;
				default:
				// assert(false);
				break;
         	}
		}
        
        assert(m_currentAnimation);
        m_currentAnimation->update(time);
    }

    void Enemy::render(gf::RenderTarget& target){
        gf::AnimatedSprite animated;
        assert(m_currentAnimation);
        animated.setAnimation(*m_currentAnimation);
		animated.setPosition(m_position);
        animated.setScale(0.75f);
        animated.setAnchor(gf::Anchor::Center);
        target.draw(animated);
    }
    

    gf::Vector2f Enemy::getPosition(){
        return m_position;
    }

    void Enemy::setPosition(gf::Vector2f position){
        m_position = position;
    }

    void Enemy::loadAnimation(gf::Animation &animation, int line) {
        static constexpr gf::Vector2f TextureSize = { 256.0f, 768.0f };
        static constexpr gf::Vector2f FrameSize = { 64.0f, 100.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/5.0f);

        for (int i = 0; i < 3; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(m_texture, frame, FrameDuration);
        }
        
    }

}
