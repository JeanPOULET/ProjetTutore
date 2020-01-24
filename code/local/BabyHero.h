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

namespace KGB{

    class BabyHero : public gf::Entity{
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

        private:
            Dynamics dynamics;
            Graphics graphics;
            
            enum class Status {
                Walking,
                Waiting,
            };

            Status m_status;
    };
}

#endif // KGB_BabyHero_H
