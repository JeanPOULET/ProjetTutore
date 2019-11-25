#include "Square.h"


namespace KGB{

    void Square::setVelocity(gf::Vector2f velocity) {
        m_velocity = velocity;
    }
    void Square::update(float dt) {
        m_position += dt * m_velocity;
    }
    void Square::render(gf::RenderTarget& target){
        gf::RectangleShape shape({ m_size, m_size });
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }

}