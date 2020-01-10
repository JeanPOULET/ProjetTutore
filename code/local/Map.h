
#ifndef KGB_MAP_H
#define KGB_MAP_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/TileLayer.h>
#include <gf/Tmx.h>

#include "Messages.h"


namespace KGB {

  struct MapGraphicsData {
    MapGraphicsData(const gf::TmxLayers& layers);

    std::vector<gf::TileLayer> tiles;
    std::vector<gf::Sprite> sprites;
  };

  class Map : public gf::Entity {
  public :
    enum Type {
      Below,
      Above,
    };

    Map(Type type, MapGraphicsData& data);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:

  private:
    Type m_type;
    MapGraphicsData& m_data;
    gf::Vector2f m_hero;
  };
}

#endif // KGB_MAP_H
