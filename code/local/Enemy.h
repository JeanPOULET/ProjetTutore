#ifndef KGB_Enemy_H
#define KGB_Enemy_H

#include <vector>

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
#include <gf/Log.h>
#include <gf/Sprite.h>
#include "ResourceManager.h"

#include "Messages.h"
#include "Singletons.h"
#include "Dynamics.h"
#include "Graphics.h"
#include "Physics.h"
#include "DataType.h"
#include "KEntity.h"

#include <Box2D/Box2D.h>

namespace KGB{

    class Enemy : public gf::Entity, public KGB::KEntity{
        public:
            enum class PathType{
                VerticalLine,
                HorizontalLine,
                Round,
                Static,
            };

            enum class Status {
                Walking,
                Waiting,
            };

            Enemy(gf::Vector2f position, PathType path, gf::Orientation ori, Status status, float distance, float speed, std::vector<gf::Orientation> rotation);

            void setVelocity(gf::Vector2f velocity);
            void update(gf::Time time);
            void render(gf::RenderTarget& target);
            gf::Vector2f getPosition();
            void setPosition(gf::Vector2f position);
            gf::Vector2f getVelocity();
            void loadAnimation(gf::Animation &animation, int line);
            void updateOrientation(int orientation);
            void defRoundStart();
            void round();
            void lineH();
            void lineV();
            void rotation();
            void timer(float seconds);
            void viewCone();
            void startContact(int contactwith);
            void endContact(int contactwith);
            void setBodyPhysics(b2World& world);
	    	void updatePhysics_set();
	    	void updatePhysics_correction();
	    	int getEntityType() { return DataType::Main_Type::ENEMY; }
		    int getSecondType() { return 0; }

            Graphics graphics;
            gf::Vector2f m_spawn;
	   	 	

        private:
            
            Dynamics dynamics;
            gf::Clock m_clock;
            size_t cpt = 0;
            std::vector<gf::Orientation> m_rotation;
            gf::Polygon m_cone;

            Status m_status;
        	b2Body *m_body;    
		
            PathType m_path;
                
    };
}

#endif // KGB_Enemy_H
