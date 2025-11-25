#pragma once

#include "Item.h"
#include "Texture.h"

class BlueDragon : public Item {
	public:
		BlueDragon() {
			texID = TEXTURE.loadTexture("./assets/east.png");
			_size = { 60.3f, 57.5f };
		}
		std::string getName() override {
			return "Â—´‚Ì‘œ";
		}
		ItemTag getTag() override {
			return ItemTag::STATUE;
		}
};