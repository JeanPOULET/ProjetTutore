
#include <gf/Entity.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>

namespace KGB{
    class Square : public gf::Entity{
        public:
            Square(gf::Vector2f position, float size, gf::Color4f color)
                : m_position(position)
                , m_velocity(0, 0)
                , m_size(size)
                , m_color(color)
                {
            }
            void setVelocity(gf::Vector2f velocity);
            gf::Vector2f getPosition();
            void update(float dt);
            void render(gf::RenderTarget& target);
        private:
            gf::Vector2f m_position; // center of the square
            gf::Vector2f m_velocity;
            float m_size;
            gf::Color4f m_color;
        };
    }

