#include "Enemy.h"
#include "Messages.h"
#include "Singletons.h"

namespace KGB{
  
    Enemy::Enemy(gf::Vector2f position)
        : m_position(position)
        , m_velocity(0, 0)
        //, m_status(Status::Waiting)
        , m_texture("../data/Image/enemy_animation.png")
        , m_orientation(gf::Orientation::South)
        , m_currentAnimation(nullptr)
        {
        loadAnimation(m_moveSouth, 0);
        loadAnimation(m_moveEast, 1);
        loadAnimation(m_moveWest, 2);
        loadAnimation(m_moveNorth, 3);
        /*loadAnimation(m_waitSouth, 4);
        loadAnimation(m_waitEast, 5);
        loadAnimation(m_waitWest, 6);
        loadAnimation(m_waitNorth, 7);*/
    }


    void Enemy::setVelocity(gf::Vector2f velocity) {
      
        m_velocity = velocity;
      
    }
/*
    void Enemy::updateOrientation(int orientation){
      switch(orientation){
        case 0:
          m_orientation =  gf::Orientation::South;
          break;
        case 1:
          m_orientation =  gf::Orientation::North;
          break;
        case 2:
          m_orientation =  gf::Orientation::East;
          break;
        case 3:
          m_orientation =  gf::Orientation::West;
          break;
        default:
        break;
      }
    }*/

    void Enemy::update(gf::Time time) {
        m_position += time.asSeconds() * m_velocity;
        
       /* if(m_velocity.x == 0 && m_velocity.y == 0){
          m_status = Status::Waiting;
        }else{
          m_status = Status::Walking;
        }

        if(m_status == Status::Walking){*/
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
        /*}else{
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
        }*/

        assert(m_currentAnimation);
        m_currentAnimation->update(time);
    }

    void Enemy::render(gf::RenderTarget& target){
        gf::AnimatedSprite animated;
        assert(m_currentAnimation);
        animated.setAnimation(*m_currentAnimation);
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
        static constexpr gf::Vector2f TextureSize = { 292.0f, 584.0f };
        static constexpr gf::Vector2f FrameSize = { 100.0f, 160.0f };
        static constexpr gf::Time FrameDuration = gf::seconds(1.0f/5.0f);

        for (int i = 0; i < 3; ++i) {
          gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
          animation.addFrame(m_texture, frame, FrameDuration);
        }
        
    }

}
