#ifndef KGB_KENTITY_H
#define KGB_KENTITY_H

#include <gf/Entity.h>

namespace KGB {
	class KEntity {
		
		public:

			virtual int getEntityType() = 0;
			virtual int getSecondType() = 0;
			//virtual bool toBeRemoved() = false;

	};
}

#endif // KGB_KENTITY_H
