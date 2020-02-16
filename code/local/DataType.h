#ifndef KGB_DATA_H
#define KGB_DATA_H

#include <gf/Entity.h>

namespace KGB {
	class DataType {
	public:

		enum Main_Type{

			BABY 			= 0x0001,
        		ENEMY 			= 0x0004,
        		OTHER 			= 0x0010,
        		HARVESTABLE 		= 0x0014,
			PROJECTILE 		= 0x0020,

		};

		enum Item_Type{

			BONUS 			= 1,
			MALUS 			= 2,

		};

		enum Bonus_Type {

            		STUNNING_DIAPERS    = 	1,
			INVISIBLE_DIAPERS   = 	2,
			SPEED_DIAPERS 	    =	3        

		};

		enum Projectile_Type {

            		HERO_STUN    = 	1,

		};

	private:

	};
}

#endif // KGB_DATA_H
