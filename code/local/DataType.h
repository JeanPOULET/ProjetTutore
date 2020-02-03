#ifndef KGB_DATA_H
#define KGB_DATA_H

#include <gf/Entity.h>

namespace KGB {
	class DataType {
	public:

		struct BodyUserData{
		
			gf::Entity *entity;
			int main_type;
			int sub_type1 = 0;
			int sub_type2 = 0;
			int sub_type3 = 0;

		};

		enum Main_Type{

			BABY = 0x0001,
        	ENEMY = 0x0004,
        	OTHER = 0x0010,
        	HARVESTABLE = 0x0014,

		};

		enum Item_Type{

			BONUS = 		1,
			MALUS = 		2,

		};

		enum Bonus_Type {

            STUNNING_DIAPERS = 	1,
        
		};

	private:

	};
}

#endif // KGB_DATA_H
