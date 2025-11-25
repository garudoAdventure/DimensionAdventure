#pragma once

#include "Item.h"
#include "Texture.h"

class RedBird : public Item {
	public:
		RedBird() {
			texID = TEXTURE.loadTexture("./assets/south.png");
			_size = { 60.3f, 57.5f };
		}
		std::string getName() override {
			return "é‚Ì‘œ";
		}
		ItemTag getTag() override {
			return ItemTag::STATUE;
		}
};