#ifndef KGB_OBJECT_H
#define KGB_OBJECT_H

#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Audio.hpp>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>


#include "Messages.h"
#include "DataType.h"
#include "Singletons.h"
#include "KEntity.h"
#include "Physics.h"

namespace KGB{

    enum ObjectType {
        CLEF  = 1,
        OTHER = 2,
        ENTRY = 3,
    };

    class Object : public gf::Entity, public KGB::KEntity {
        public:
            Object(ObjectType type, gf::Vector2f pos, gf::Sprite sprite);

            ObjectType m_objectType;
            gf::Vector2f m_position;
            gf::Sprite m_sprite;
            void startContact(int contactwith);
            int getEntityType(){return m_objectType;};
	        int getSecondType(){return 0;};
            void setObjectBody(b2World& world, gf::Vector2f position);
            void render(gf::RenderTarget& target);
            void update(gf::Time time) override;
            void removeBody();
            b2Body *m_body;
            bool alive=true;
    };
}


#endif //KGB_OBJECT_H
