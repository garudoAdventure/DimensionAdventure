#pragma once

#include "GameObj.h"
#include "Player.h"
#include "Texture.h"
#include "Sprite.h"
#include <string>

class Item {
	public:
		Item() = default;
		virtual void draw3D(Float3 pos) {
			SPRITE.drawSpriteIn3D(pos, _size, texID);
		}
		virtual void draw2D(Float2 pos) {
			SPRITE.drawSprite2D(pos, _size, texID);
		}
		virtual void get(Player* player) = 0;
		virtual std::string getName() = 0;

	protected:
		unsigned int texID;
		Float2 _size;
};