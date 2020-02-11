#ifndef KGB_Graphics_H
#define KGB_Graphics_H

#include <gf/Animation.h>
#include <gf/Orientation.h>
#include <gf/AnimatedSprite.h>

namespace KGB {
    struct Graphics {
        gf::Texture *m_texture;
        gf::Orientation m_orientation;
        gf::Orientation m_originaleOrientation;

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
    };
}
#endif // KGB_Graphics_H