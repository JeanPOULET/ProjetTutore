#ifndef KGB_OBJECTS_H
#define KGB_OBJECTS_H

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
        CLEF = 1
    };

    class object : public gf::Entity, public KGB::KEntity {
        public:
        object(ObjectType type, gf::Vector2f pos, gf::Sprite sprite)
        : gf::Entity(10)
        , m_objectType(type)
        , m_position(pos)
        , m_sprite(sprite)
        , m_body(nullptr)
        {

        }

        ObjectType m_objectType;
        gf::Vector2f m_position;
        gf::Sprite m_sprite;
        void startContact(int contactwith);
        int getEntityType(){return m_objectType;};
        b2Body *m_body;
    };

    class Objects : public gf::Entity, public KGB::KEntity{

        public :
            Objects(); 
            virtual void update(gf::Time time) override;
            virtual void render(gf::RenderTarget& target);
            void addObject(ObjectType type, gf::Vector2f position, gf::Sprite sprite);
            void startContact(int contactwith);
            int getEntityType(){return ObjectType::CLEF;};
            void setObjectBody(int num, b2World& world, gf::Vector2f position);

        private:
            std::vector<object> m_objects;
            

    };
}








#endif //KGB_OBJECTS_H