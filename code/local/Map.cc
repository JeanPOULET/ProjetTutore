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
      LayersMaker(std::vector<gf::TileLayer>& layers, std::vector<gf::Sprite>& sprites, std::vector<Object>& objs)
      : m_layers(layers)
      , m_sprites(sprites)
      , m_objs(objs)
      {

      }

      virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
			if (!layer.visible) {
				return;
			}

			assert(map.orientation == gf::TmxOrientation::Orthogonal);

			gf::TileLayer tileLayer(map.mapSize, gf::TileLayer::Orthogonal);

			gf::Log::info("Parsing layer '%s'\n", layer.name.c_str());

			tileLayer.setTileSize(map.tileSize);

			int k = 0;

			for (auto& cell : layer.cells) {
				int i = k % map.mapSize.width;
				int j = k / map.mapSize.width;
				assert(j < map.mapSize.height);

				int gid = cell.gid;
		

				if (gid != 0) {
					auto tileset = map.getTileSetFromGID(gid);
					assert(tileset);
					tileLayer.setTilesetTileSize(tileset->tileSize);
					tileLayer.setOffset(tileset->offset);
					tileLayer.setMargin(tileset->margin);
					tileLayer.setSpacing(tileset->spacing);
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
		gf::Log::info("Parsing Object& layer '%s'\n", layer.name.c_str());


		for (auto& Object : layer.objects) {
			if (Object->kind != gf::TmxObject::Tile) {
				continue;
			}

			auto tile = static_cast<gf::TmxTileObject *>(Object.get());

			auto tileset = map.getTileSetFromGID(tile->gid);
			assert(tileset);
			assert(tileset->image);
			

			auto lid = tile->gid - tileset->firstGid;
			auto subTexture = tileset->getSubTexture(lid, tileset->image->size);
			
			constexpr gf::Vector2f vecclef_posRandom0 = {2240,2012}; //Salle à manger
			constexpr gf::Vector2f vecclef_posRandom1 = {1025,2213}; //Salle de jeu
			constexpr gf::Vector2f vecclef_posRandom2 = {2569,222};  //Bibliothèque
			std::vector<gf::Vector2f> randomClefPositions ={vecclef_posRandom0,vecclef_posRandom1,vecclef_posRandom2};
			
			gf::Vector2f position = tile->position;
			
			position.x += subTexture.getWidth() / 2;
			position.y -= subTexture.getHeight() / 2;
			if(layer.name == "Objets"){
				gf::Texture& texture = gResourceManager().getTexture(tileset->image->source);
				gf::RectF textureRect = texture.computeTextureCoords(subTexture);

				gf::Sprite sprite(texture, textureRect);
				ObjectType type;
				switch(lid){
					case 1055 : //CLEF
                   		position = randomClefPositions[gRandom().computeUniformInteger(0,2)];
            			type = ObjectType::CLEF;
					break;
					case 552 : 
						type = ObjectType::ENTRY;
					break;
					case 553 : 
						type = ObjectType::ENTRY;
					break;
					case 554 : 
						type = ObjectType::ENTRY;
					break;
					default :
						type = ObjectType::OTHER;
					break;
				}
				KGB::Object ob(type,position,sprite);
				m_objs.push_back(ob);
				gf::Log::debug("lid = %d\n",lid);

			}else{
				const gf::Texture& texture = gResourceManager().getTexture(tileset->image->source);
				gf::RectF textureRect = texture.computeTextureCoords(subTexture);

				gf::Sprite sprite(texture, textureRect);
				sprite.setPosition(tile->position);
				sprite.setRotation(gf::degreesToRadians(tile->rotation));
				sprite.setAnchor(gf::Anchor::BottomLeft); 

				m_sprites.push_back(std::move(sprite));
			}
		}
    }

		private:
			std::vector<gf::TileLayer>& m_layers;
			std::vector<gf::Sprite>& m_sprites;
			std::vector<Object>& m_objs;
		};
	}

	MapGraphicsData::MapGraphicsData(const gf::TmxLayers& layers, std::vector<Object>& objs)
	{
		LayersMaker maker(tiles, sprites,objs);
		layers.visitLayers(maker);
	}

	Map::Map(MapGraphicsData& data)
	: gf::Entity(0)
	, m_data(data)
	{

	}

	void Map::render(gf::RenderTarget& target, const gf::RenderStates& states) {

		for (auto& layer : m_data.tiles) {
			target.draw(layer,states);
		}
		
		for (auto& sprite : m_data.sprites) {
			target.draw(sprite,states);

		}
		
	}

}
