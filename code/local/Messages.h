
#ifndef KGB_MESSAGES_H
#define KGB_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

using namespace gf::literals;

namespace KGB{

    struct GameOver: public gf::Message {
        static const gf::Id type = "GameOver"_id;
    };

    struct Victory: public gf::Message {
        static const gf::Id type = "Victory"_id;
    };

    struct Clef: public gf::Message {
        static const gf::Id type = "Clef"_id;
    };
}

#endif //KGB_MESSAGES_H