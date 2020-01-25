#ifndef KGB_Enemy_H
#define KGB_Enemy_H

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

#include "Messages.h"
#include "Singletons.h"
#include "Dynamics.h"
#include "Graphics.h"

#include <Box2D/Box2D.h>

namespace KGB{

    class Enemy : public gf::Entity{
        public:
            enum class PathType{
                VerticalLine,
                HorizontalLine,
                Round,
                Static,
            };

            enum class Status {
                Walking,
                Waiting,
            };

            Enemy(gf::Vector2f position, PathType path, gf::Orientation ori, Status status);

            void setVelocity(gf::Vector2f velocity);
            void update(gf::Time time);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            void setPosition(gf::Vector2f position);
            gf::Vector2f getVelocity();
            void loadAnimation(gf::Animation &animation, int line);
            void updateOrientation(int orientation);
            void round();
            void lineH();
            void lineV();
            void viewCone();

        private:
            gf::Vector2f m_spawn;
            Dynamics dynamics;
            Graphics graphics;
            
            
            gf::Polygon m_cone;

            Status m_status;
            
            PathType m_path;
                
    };
}

#endif // KGB_Enemy_H
