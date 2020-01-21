#ifndef Item_H
#define Item_H

#include <gf/Entity.h>
#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Activities.h>
#include <gf/Move.h>
#include <gf/Anchor.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>
#include <gf/Sprite.h>
#include <gf/Animation.h>
#include <gf/Orientation.h>
#include <gf/AnimatedSprite.h>

#include "Messages.h"
#include "Singletons.h"

namespace KGB{

    enum class Status {
                Moving,
                Waiting,
            };
    enum class ItemType {
                Munition,
		Vie
            };

    class Item : public gf::Entity{
        public:
            Item(gf::Vector2f position, Status stat = Status::Waiting, ItemType type = ItemType::Munition);

            void setVelocity(gf::Vector2f velocity);
			gf::Vector2f getVelocity();
            void update(gf::Time time);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            void setPosition(gf::Vector2f position);
            void loadAnimation(gf::Animation &animation, int line);
            void updateOrientation(int orientation);


        private:
            gf::Vector2f m_position; // center of the BabyHero
            gf::Vector2f m_velocity;
            gf::Texture &m_texture;
            gf::Orientation m_orientation;

            //Animation
            gf::Animation *m_currentAnimation;
            
            gf::Animation m_moveWest;
            gf::Animation m_moveEast;
            gf::Animation m_moveNorth;
            gf::Animation m_moveSouth;

            gf::Animation m_waitWest;
            gf::Animation m_waitEast;
            gf::Animation m_waitNorth;
            gf::Animation m_waitSouth;
            
            Status m_status;
    };
}

#endif // Item_H
