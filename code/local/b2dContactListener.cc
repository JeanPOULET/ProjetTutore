#include "b2dContactListener.h"

namespace KGB{
	
	void b2dContactListener::BeginContact(b2Contact* contact){

		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();
		
		
		
		if(dataA && dataB){

			KGB::KEntity *dataTypeA = static_cast<KGB::KEntity*>(dataA);
			KGB::KEntity *dataTypeB = static_cast<KGB::KEntity*>(dataB);

			gf::Log::debug("avant entA\n");
			int entityTypeA = dataTypeA->getEntityType();
			gf::Log::debug("après ent A\n");
			int entityTypeB = dataTypeB->getEntityType();

			
			//Element A
			
			if(entityTypeA == DataType::Main_Type::BABY) {
		
				BabyHero* bb = static_cast<BabyHero*>(dataTypeA);
				if(bb){
					bb->startContact(entityTypeB);
				}
		   	}
			   
			if(entityTypeA == DataType::Main_Type::ENEMY) {
		
				Enemy* enemy = static_cast<Enemy*>(dataTypeA);
				if(enemy){
					enemy->startContact(entityTypeB);
				}
		   	}
			   
			if(entityTypeA == DataType::Main_Type::HARVESTABLE) {
		
				Bonus* bonus = static_cast<Bonus*>(dataTypeA);
				if(bonus){
					bonus->startContact(entityTypeB);
				}
		   	} 

			if(entityTypeA == ObjectType::CLEF) {
		
				Object* obj = static_cast<Object*>(dataTypeA);
				if(obj){
					obj->startContact(entityTypeB);
				}
		   	}
			if(entityTypeA == DataType::Main_Type::ENTRY ){
				gf::Log::debug("Enti A\n");
				Entry* ent = static_cast<Entry*>(dataTypeA);
				if(ent){
					ent->startContact(entityTypeB);
				}								

			}
			
			//Element B
			
			if(entityTypeB == DataType::Main_Type::BABY ) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeB);
				if(bb){
					bb->startContact(entityTypeA);
				}
		   	}
			
			if(entityTypeB == DataType::Main_Type::ENEMY ) {
		
				Enemy* enemy = static_cast<Enemy*>(dataTypeB);
				if(enemy){
					enemy->startContact(entityTypeA);
				}
		   	}

			if(entityTypeB == ObjectType::CLEF) {
		
				Object* obj = static_cast<Object*>(dataTypeB);
				if(obj){
					obj->startContact(entityTypeA );
				}
		   	} 
			
			if(entityTypeB == DataType::Main_Type::HARVESTABLE) {

				Bonus* bonus = static_cast<Bonus*>(dataTypeB);
				if(bonus){
					bonus->startContact(entityTypeA);
				}
		   	}

			if(entityTypeB == DataType::Main_Type::ENTRY ){
				gf::Log::debug("Enti B\n");
				Entry* ent = static_cast<Entry*>(dataTypeB);
				if(ent){
					ent->startContact(entityTypeA);
				}								

			}
			   
		}
		
    }

    void b2dContactListener::EndContact(b2Contact* contact) {
		
		/*void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();
		
		if(dataA && dataB){
			
			KGB::KEntity *dataTypeA = static_cast<KGB::KEntity*>(dataA);
			KGB::KEntity *dataTypeB = static_cast<KGB::KEntity*>(dataB);
			
			int entityTypeA = dataTypeA->getEntityType();
			int entityTypeB = dataTypeB->getEntityType();
			
			//Element A
			
			if(entityTypeA == DataType::Main_Type::BABY) {
		
				BabyHero* bb = static_cast<BabyHero*>(dataTypeA);
				if(bb){
					bb->endContact(entityTypeB);
				}
		   	}
			   
			if(entityTypeA == DataType::Main_Type::ENEMY) {
		
				Enemy* enemy = static_cast<Enemy*>(dataTypeA);
				if(enemy){
					enemy->endContact(entityTypeB);
				}
		   	}
			   
			  
			
			//Element B
			
			if(entityTypeB == DataType::Main_Type::BABY ) {
		
				BabyHero* bb = static_cast<BabyHero*> (dataTypeB);
				if(bb){
					bb->endContact(entityTypeA);
				}
		   	}
			
			if(entityTypeB == DataType::Main_Type::ENEMY ) {
		
				Enemy* enemy = static_cast<Enemy*>(dataTypeB);
				if(enemy){
					enemy->endContact(entityTypeA);
				}
		   	}
			
		}
*/
    }
}
