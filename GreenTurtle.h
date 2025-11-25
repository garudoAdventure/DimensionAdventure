#pragma once

#include "Item.h"
#include "Texture.h"

class GreenTurtle : public Item {
	public:
		GreenTurtle() {
			texID = TEXTURE.loadTexture("./assets/north.png");
			_size = { 60.3f, 57.5f };
		}
		std::string getName() override {
			return "Œº•‚Ì‘œ";
		}
		ItemTag getTag() override {
			return ItemTag::STATUE;
		}
};