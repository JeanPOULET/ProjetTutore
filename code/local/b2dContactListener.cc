#include "b2dContactListener.h"

namespace KGB{
	void b2dContactListener::BeginContact(b2Contact* contact){

		bool isSensorA = contact->GetFixtureA()->IsSensor();
		bool isSensorB = contact->GetFixtureB()->IsSensor();
		void* bodyUserData;
		if(isSensorA && !isSensorB){
			bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		}else if(isSensorB && !isSensorA){
			bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		}else{
			return;
		}
		/*BabyHero* bb = static_cast<BabyHero*> (bodyUserData);
		bb->startContact();*/
        /*;
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
		}*/
    }

    void b2dContactListener::EndContact(b2Contact* contact) {
		bool isSensorA = contact->GetFixtureA()->IsSensor();
		bool isSensorB = contact->GetFixtureB()->IsSensor();
		void* bodyUserData;
		if(isSensorA && !isSensorB){
			bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		}else if(isSensorB && !isSensorA){
			bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		}else{
			return;
		}
		/*BabyHero* bb = static_cast<BabyHero*> (bodyUserData);
		bb->endContact();*/
  
		/*void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if ( bodyUserData ){
			static_cast<BabyHero*>( bodyUserData )->endContact();

		}
	
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if ( bodyUserData ){
			static_cast<BabyHero*>( bodyUserData )->endContact();
		}*/
  
    }
}
