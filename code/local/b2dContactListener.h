#ifndef KGB_b2dContactListener_H
#define KGB_b2dContactListener_H

#include <Box2D/Box2D.h>
#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Model.h>
#include <gf/Polygon.h>
#include <gf/Tmx.h>
#include <stdio.h>

#include <set>
#include <string>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Polyline.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "BabyHero.h"
#include "Enemy.h"
#include "Debug.h"
#include "Square.h"
#include "Physics.h"


namespace KGB{
    class b2dContactListener : public b2ContactListener{
        public:

            // Called when two fixtures begin to touch
            virtual void BeginContact(b2Contact* contact);
        
            // Called when two fixtures cease to touch
            virtual void EndContact(b2Contact* contact);
    };

    
        
}

#endif