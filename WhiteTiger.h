#pragma once

#include "Item.h"
#include "Texture.h"

class WhiteTiger : public Item {
	public:
		WhiteTiger() {
			texID = TEXTURE.loadTexture("./assets/west.png");
			_size = { 60.3f, 57.5f };
		}
		std::string getName() override {
			return "”’ŒÕ‚Ì‘œ";
		}
		ItemTag getTag() override {
			return ItemTag::STATUE;
		}
};