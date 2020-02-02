#include "Enemy.h"


namespace KGB{
  
	Enemy::Enemy(gf::Vector2f position, PathType path, gf::Orientation ori, Status status, float distance)
        : m_spawn(position)
		, m_path(path)
		, m_status(status)
		, m_body(nullptr)
		, m_cone(nullptr)
		, m_distance(distance)
        {
		
		dynamics.m_position = position;
        dynamics.m_velocity = gf::Vector2f(0, 0);
        graphics.m_texture = &gResourceManager().getTexture("Image/Polizei_animation.png");
        graphics.m_orientation = ori;
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
		if(dynamics.m_position.x <= m_spawn.x && dynamics.m_position.y >= m_spawn.y + m_distance){
			graphics.m_orientation = gf::Orientation::East;
			dynamics.m_velocity = gf::Vector2f(50.0,0);
		}else if(dynamics.m_position.x >= m_spawn.x + m_distance && dynamics.m_position.y >= m_spawn.y + m_distance){
			graphics.m_orientation = gf::Orientation::North;
			dynamics.m_velocity = gf::Vector2f(0,-50.0);
		}else if(dynamics.m_position.x >= m_spawn.x + m_distance && dynamics.m_position.y <= m_spawn.y){
			graphics.m_orientation = gf::Orientation::West;
			dynamics.m_velocity = gf::Vector2f(-50.0,0);
		}else if(dynamics.m_position.x <= m_spawn.x && dynamics.m_position.y <= m_spawn.y){
			graphics.m_orientation = gf::Orientation::South;
			dynamics.m_velocity = gf::Vector2f(0,50.0);
		}
	}

	void Enemy::lineH(){
		if(dynamics.m_position.x <= m_spawn.x){
			graphics.m_orientation = gf::Orientation::East;
			dynamics.m_velocity = gf::Vector2f(50.0,0);
		}else if(dynamics.m_position.x >= m_spawn.x + m_distance){
			graphics.m_orientation = gf::Orientation::West;
			dynamics.m_velocity = gf::Vector2f(-50.0,0);
		}
	}

	void Enemy::lineV(){
		if(dynamics.m_position.y >= m_spawn.y + m_distance){
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
		gf::Polygon m_cone;
		if(this->graphics.m_orientation==gf::Orientation::South){
			m_cone.addPoint({10.0f,10.0f});
			m_cone.addPoint({20.0f,200.0f});
			m_cone.addPoint({-20.0f,200.0f});
		}else if(this->graphics.m_orientation==gf::Orientation::North){
			m_cone.addPoint({0.0f,0.0f});
			m_cone.addPoint({20.0f,-200.0f});
			m_cone.addPoint({-20.0f,-200.0f});
		}else if(this->graphics.m_orientation==gf::Orientation::East){
			m_cone.addPoint({-10.0f,10.0f});
			m_cone.addPoint({100.0f,150.0f});
			m_cone.addPoint({100.0f,-150.0f});
		}else{
			m_cone.addPoint({-40.0f,10.0f});
			m_cone.addPoint({-100.0f,150.0f});
			m_cone.addPoint({-100.0f,-150.0f});			
		}
		gf::ConvexShape coneShape(m_cone);
		
		coneShape.setPosition({dynamics.m_position.x,dynamics.m_position.y});
        coneShape.setColor(gf::Color::Black);
        coneShape.setAnchor(gf::Anchor::Center);

        gf::AnimatedSprite animated;
        assert(graphics.m_currentAnimation);
        animated.setAnimation(*graphics.m_currentAnimation);
		animated.setPosition(dynamics.m_position);
        animated.setScale(0.75f);
        animated.setAnchor(gf::Anchor::Center);
        target.draw(animated);
		//target.draw(coneShape);
    }
    

    gf::Vector2f Enemy::getPosition(){
        return dynamics.m_position;
    }

    void Enemy::setPosition(gf::Vector2f position){
        dynamics.m_position = position;
    }

    void Enemy::loadAnimation(gf::Animation &animation, int line) {
        static constexpr gf::Vector2f TextureSize = { 256.0f, 768.0f };
        static constexpr gf::Vector2f FrameSize = { 64.0f, 96.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/5.0f);

        for (int i = 0; i < 4; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(*graphics.m_texture, frame, FrameDuration);
        }
    }

    void Enemy::startContact() {
      	gf::Log::info("AROUF GANGSTA\n");
    }
    void Enemy::endContact() { 
		gf::Log::info("JE SUIS LE ROI ARROUF\n");
	}
	
	void Enemy::setBodyPhysics(b2World& world){

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Physics::fromVec(this->getPosition());
		m_body = world.CreateBody(&bodyDef);

		/*DataType::BodyUserData enemy;
		enemy.entity = this;
		enemy.main_type = DataType::Main_Type::ENEMY;*/

		m_body->SetUserData((void*) static_cast<KGB::KEntity*>(this));

		b2PolygonShape shapeEnemy;
		shapeEnemy.SetAsBox(15.0f*Physics::getPhysicScale(), 18.0f*Physics::getPhysicScale());

		b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = DataType::Main_Type::ENEMY;
        fixtureDef.shape = &shapeEnemy;

		m_body->CreateFixture(&fixtureDef);
		
		//cone
		
		b2FixtureDef fixtureCone;
        b2PolygonShape shapeCone;
        b2Vec2 vertices[3];
        vertices[0].Set(0,0);
        vertices[1].Set(-20* Physics::getPhysicScale(), 200* Physics::getPhysicScale());
        vertices[2].Set( 20* Physics::getPhysicScale(), 200* Physics::getPhysicScale());

        shapeCone.Set(vertices, 3);

        fixtureCone.density = 1.0f;
        fixtureCone.friction = 0.0f;
        fixtureCone.restitution = 0.0f;
        fixtureCone.isSensor = true;
        fixtureCone.filter.categoryBits = DataType::Main_Type::ENEMY;
        fixtureCone.filter.maskBits = DataType::Main_Type::BABY;
        fixtureCone.shape = &shapeCone;

        m_body->CreateFixture(&fixtureCone);

    }

    void Enemy::updatePhysics_set(){

	m_body->SetTransform(Physics::fromVec(this->getPosition()), 0.0f);
	m_body->SetLinearVelocity(Physics::fromVec(this->getVelocity()));

    }

    void Enemy::updatePhysics_correction(){

	setPosition(Physics::toVec(m_body->GetPosition()));

    }

}
