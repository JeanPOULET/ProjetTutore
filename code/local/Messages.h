
#ifndef KGB_MESSAGES_H
#define KGB_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

using namespace gf::literals;

namespace KGB{

    struct SquareState : public gf::Message{
        static constexpr gf::Id type ="Square"_id;

        gf::Vector2f position;


    };


}

#endif //KGB_MESSAGES_H