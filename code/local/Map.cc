#include "Map.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Tmx.h>

#include "Singletons.h"

namespace KGB {

  namespace {

    class LayersMaker : public gf::TmxVisitor {
    public:
      LayersMaker(std::vector<gf::TileLayer>& layers, std::vector<gf::Sprite>& sprites)
      : m_layers(layers)
      , m_sprites(sprites)
      {

      }

      virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
        if (!layer.visible) {
          return;
        }

        //assert(map.orientation == gf::TmxOrientation::Staggered);

        gf::TileLayer tileLayer(map.mapSize, gf::TileLayer::Orthogonal);

        gf::Log::info("Parsing layer '%s'\n", layer.name.c_str());

        tileLayer.setTileSize(map.tileSize);

        int k = 0;

        for (auto& cell : layer.cells) {
          int i = k % map.mapSize.width;
          int j = k / map.mapSize.width;
          assert(j < map.mapSize.height);

          int gid = cell.gid;
          gf::Log::info("cell.gid %d\n", gid);

          if (gid != 0) {
            auto tileset = map.getTileSetFromGID(gid);
            assert(tileset);
            tileLayer.setTilesetTileSize(tileset->tileSize);
            tileLayer.setOffset(tileset->offset);

            gid = gid - tileset->firstGid;
            tileLayer.setTile({ i, j }, gid, cell.flip);

            if (!tileLayer.hasTexture()) {
              assert(tileset->image);
              const gf::Texture& texture = gResourceManager().getTexture(tileset->image->source);
              tileLayer.setTexture(texture);
            } else {
              assert(&gResourceManager().getTexture(tileset->image->source) == &tileLayer.getTexture());
            }

          }

          k++;
        }

        m_layers.push_back(std::move(tileLayer));
      }

      virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
        gf::Log::info("Parsing object layer '%s'\n", layer.name.c_str());


        for (auto& object : layer.objects) {
          if (object->kind != gf::TmxObject::Tile) {
            continue;
          }

          auto tile = static_cast<gf::TmxTileObject *>(object.get());

          auto tileset = map.getTileSetFromGID(tile->gid);
          assert(tileset);
          assert(tileset->image);


          auto lid = tile->gid - tileset->firstGid;
          auto subTexture = tileset->getSubTexture(lid, tileset->image->size);
          
          const gf::Texture& texture = gResourceManager().getTexture(tileset->image->source);
          gf::RectF textureRect = texture.computeTextureCoords(subTexture);

          gf::Sprite sprite(texture, textureRect);
          sprite.setPosition(tile->position);
          sprite.setRotation(gf::degreesToRadians(tile->rotation));
          sprite.setAnchor(gf::Anchor::BottomLeft); 

          m_sprites.push_back(std::move(sprite));
          
        }
      }

    private:
      std::vector<gf::TileLayer>& m_layers;
      std::vector<gf::Sprite>& m_sprites;
    };
  }

  MapGraphicsData::MapGraphicsData(const gf::TmxLayers& layers)
  {
    LayersMaker maker(tiles, sprites);
    layers.visitLayers(maker);
  }

  Map::Map(MapGraphicsData& data)
  : gf::Entity(0)
  , m_data(data)
  {
     //gMessageManager().registerHandler<HeroPosition>(&Map::onHeroPosition, this);
  }

  void Map::render(gf::RenderTarget& target) {
      //, const gf::RenderStates& states
      for (auto& layer : m_data.tiles) {
        target.draw(layer);
      }

      for (auto& sprite : m_data.sprites) {
        target.draw(sprite);

      }

      for (auto& sprite : m_data.sprites) {
          target.draw(sprite);
      }
    
  }

}
