#include "b2dContactListener.h"

namespace KGB{
	
	void b2dContactListener::BeginContact(b2Contact* contact){

		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();
		
		if(dataA && dataB){
			
			KGB::KEntity *dataTypeA = static_cast<KGB::KEntity*>(dataA);
			KGB::KEntity *dataTypeB = static_cast<KGB::KEntity*>(dataB);
			
			//BABY-ENEMY
			
			if((dataTypeA->getEntityType() == DataType::Main_Type::BABY && dataTypeB->getEntityType() == DataType::Main_Type::ENEMY)) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeA);
				if(bb){
					bb->startContact();
				}
		   	}
			
			if((dataTypeB->getEntityType() == DataType::Main_Type::BABY && dataTypeA->getEntityType() == DataType::Main_Type::ENEMY)) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeB);
				if(bb){
					bb->startContact();
				}
		   	}
			
		}
		
    }

    void b2dContactListener::EndContact(b2Contact* contact) {

		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();
		
		if(dataA && dataB){
			
			KGB::KEntity *dataTypeA = static_cast<KGB::KEntity*>(dataA);
			KGB::KEntity *dataTypeB = static_cast<KGB::KEntity*>(dataB);
			
			//BABY-ENEMY
			
			if((dataTypeA->getEntityType() == DataType::Main_Type::BABY && dataTypeB->getEntityType() == DataType::Main_Type::ENEMY)) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeA);
				if(bb){
					bb->endContact();
				}
		   	}
			
			if((dataTypeB->getEntityType() == DataType::Main_Type::BABY && dataTypeA->getEntityType() == DataType::Main_Type::ENEMY)) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeB);
				if(bb){
					bb->endContact();
				}
		   	}
			
		}
  
    }
}
