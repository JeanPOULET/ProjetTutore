#include "Objects.h"

namespace KGB{
    Objects::Objects()
        : gf::Entity(10)
        {

        }


	void Objects::render(gf::RenderTarget& target){
		/*static constexpr gf::Vector2f AssetSize = { 32.0f, 32.0f };
		static constexpr gf::Vector2f TilesetSize = { 256.0f, 4256.0f };*/
    	for (auto &obj: m_objects) {
			//gf::RectF textureRect = gf::RectF::fromPositionSize({ static_cast<int>(obj.m_objectType) * AssetSize.width / TilesetSize.width, 0.0f }, AssetSize / TilesetSize);
			obj.m_sprite.setPosition(obj.m_position);
			obj.m_sprite.setAnchor(gf::Anchor::Center);
			target.draw(obj.m_sprite);
		}
    }
    

    void Objects::update(gf::Time time){

    }

	void Objects::addObject(ObjectType type, gf::Vector2f position, gf::Sprite sprite){
		m_objects.push_back({type,position,sprite});
	}
}