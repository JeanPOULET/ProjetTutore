#ifndef KGB_ENTRY_H
#define KGB_ENTRY_H
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
#include "Physics.h"
#include "DataType.h"
#include "KEntity.h"

#include <Box2D/Box2D.h>

namespace KGB{
    class Entry : public gf::Entity, public KGB::KEntity{
        public:
            Entry(gf::Vector2f position);

            void startContact(int contactwith);
            void setObjectBody(b2World& world, gf::Vector2f position);
            gf::Vector2f m_position;
            b2Body *m_body;
            void update(gf::Time time);
            int getEntityType() { return DataType::Main_Type::ENTRY;};

    };
    
}


#endif //KGB_ENTRY_H