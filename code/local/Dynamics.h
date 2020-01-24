#ifndef KGB_Dynamics_H
#define KGB_Dynamics_H

#include <gf/Vector.h>

namespace KGB {
    struct Dynamics {
        gf::Vector2f m_position;
        gf::Vector2f m_velocity;
    };
}
#endif // KGB_Dynamics_H