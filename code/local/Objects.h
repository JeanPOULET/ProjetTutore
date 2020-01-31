#ifndef KGB_OBJECTS_H
#define KGB_OBJECTS_H

#include <vector>

#include <SFML/Audio.hpp>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>


#include "Messages.h"
#include "Singletons.h"

namespace KGB{

    enum ObjectType {
        CLEF = 1056
    };

    struct object{
        object(ObjectType type, gf::Vector2f pos, gf::Sprite sprite)
        : m_objectType(type)
        , m_position(pos)
        , m_sprite(sprite)
        {

        }

        ObjectType m_objectType;
        gf::Vector2f m_position;
        gf::Sprite m_sprite;
    };

    class Objects : public gf::Entity{

        public :
            Objects(); 
            virtual void update(gf::Time time) override;
            virtual void render(gf::RenderTarget& target);
            void addObject(ObjectType type, gf::Vector2f position, gf::Sprite sprite);

        private:
            std::vector<object> m_objects;
            

    };
}








#endif //KGB_OBJECTS_H