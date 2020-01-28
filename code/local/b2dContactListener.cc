#include "b2dContactListener.h"

namespace KGB{
	void b2dContactListener::BeginContact(b2Contact* contact){
        void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if ( bodyUserData){
			BabyHero* bb = static_cast<BabyHero*> (bodyUserData);
			if(bb->getClassName()=="BabyHero"){
				bb->startContact();
			}
        }

    	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if ( bodyUserData ){
			BabyHero* bb = static_cast<BabyHero*> (bodyUserData);
			if(bb->getClassName()=="BabyHero"){
				bb->startContact();
			}
		}
    }

    void b2dContactListener::EndContact(b2Contact* contact) {
  
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if ( bodyUserData ){
			static_cast<BabyHero*>( bodyUserData )->endContact();

		}
	
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if ( bodyUserData ){
			static_cast<BabyHero*>( bodyUserData )->endContact();
		}
  
    }
}