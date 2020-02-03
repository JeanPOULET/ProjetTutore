
#ifndef KGB_MAP_H
#define KGB_MAP_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/TileLayer.h>
#include <gf/Tmx.h>

#include "Messages.h"
#include "Objects.h"

namespace KGB {

	struct MapGraphicsData {

		MapGraphicsData(const gf::TmxLayers& layers, Objects& objs);
		std::vector<gf::TileLayer> tiles;
		std::vector<gf::Sprite> sprites;
		
	};

	class Map : public gf::Entity {
		public:

			Map(MapGraphicsData& data);
			virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) ;

		private:

			MapGraphicsData& m_data;
			gf::Vector2f m_hero;
	};
}

#endif // KGB_MAP_H
