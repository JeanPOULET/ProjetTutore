#ifndef KGB_BONUS_H
#define KGB_BONUS_H

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
#include "Dynamics.h"
#include "Graphics.h"
#include "Physics.h"
#include "DataType.h"
#include "KEntity.h"

#include <Box2D/Box2D.h>

namespace KGB{

    class Bonus : public gf::Entity, public KGB::KEntity{
        public:

            enum class Status {
                Moving,
                Waiting,
            };            

            Bonus(gf::Vector2f position, DataType::Bonus_Type type);
            void setVelocity(gf::Vector2f velocity);
            gf::Vector2f getVelocity();
            void setBodyPhysics(b2World& world);
            void updatePhysics_set();
            void updatePhysics_correction();
            void update(gf::Time time);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            void setPosition(gf::Vector2f position);
            void loadAnimation(gf::Animation &animation, int line);
            void startContact(int contactwith);
            void endContact(int contactwith);
            int getEntityType() { return DataType::Main_Type::HARVESTABLE;};
            
        private:

            Dynamics dynamics;
            Graphics graphics;
            Status m_status;
	    b2Body *m_body;
	    b2Fixture *m_fixture;
	    bool remove = false;
	    bool removed = false;
    };
}

#endif // KGB_BONUS_H
