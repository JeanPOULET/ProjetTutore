#ifndef KGB_BabyHero_H
#define KGB_BabyHero_H

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

    class BabyHero : public gf::Entity, public KGB::KEntity{
        public:

            BabyHero(gf::Vector2f position);

            void setVelocity(gf::Vector2f velocity);
			gf::Vector2f getVelocity();
            void update(gf::Time time);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            void setPosition(gf::Vector2f position);
            void loadAnimation(gf::Animation &animation, int line);
            void updateOrientation(int orientation);
            std::string getClassName();
            virtual void startContact(int contactwith, int filter);
            virtual void endContact(int contactwith, int filter);
            void setBodyPhysics(b2World& world);
            void updatePhysics_set();
            void updatePhysics_correction();
            int getEntityType() { return DataType::Main_Type::BABY; }
            int getSecondType() { return 0; }
            void setInvisible(int time);
            void setSpeed(int time);
			void setStun();
            int getNbStunning();
            bool getSpeedActive();

        private:
            Dynamics dynamics;
            Graphics graphics;
            bool libre =false;
            int invi_muni = 1;
            int speed_muni = 1;
            int stun_muni = 1;

            bool invi_active = false;
            bool speed_active = false;
		
            enum class Status {
                Walking,
                Waiting,
                Immolate,
            };

            Status m_status;
	        b2Body *m_body;
            sf::Sound m_WalkingSound;
            int tempoSound;

	        std::vector<int> m_ennemycontact;
	
	        int timeout = 0;
    };
}

#endif // KGB_BabyHero_H
