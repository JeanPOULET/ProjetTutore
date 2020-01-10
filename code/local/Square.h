#ifndef KGB_SQUARE_H
#define KGB_SQUARE_H

#include <gf/Entity.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Activities.h>
#include <gf/Move.h>
#include <gf/Anchor.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>



namespace KGB{

    class Square : public gf::Entity{
        public:
            Square(gf::Vector2f position, float size, gf::Color4f color);

            void setVelocity(gf::Vector2f velocity);
            void update(float dt);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            
	    void setPosition(gf::Vector2f position);

        private:
            gf::Vector2f m_position; // center of the square
            gf::Vector2f m_velocity;
            float m_size;
            gf::Color4f m_color;
    };
}

#endif // KGB_SQUARE_H
