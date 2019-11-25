#include "Square.h"
#include "Messages.h"
#include "Singletons.h"

namespace KGB{

    Square::Square(gf::Vector2f position, float size, gf::Color4f color)
        : m_position(position)
        , m_velocity(0, 0)
        , m_size(size)
        , m_color(color)
        {
    }


    void Square::setVelocity(gf::Vector2f velocity) {
        m_velocity = velocity;
    }
    void Square::update(float dt) {
        m_position += dt * m_velocity;
        /*SquareState message;
        message.position = m_position;
        gMessageManager().sendMessage(&message);*/


    }

    void Square::render(gf::RenderTarget& target){
        gf::RectangleShape shape({ m_size, m_size });
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }
    

     gf::Vector2f Square::getPosition(){
         return m_position;
     }

}