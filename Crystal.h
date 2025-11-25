#pragma once

#include "Item.h"

class Crystal : public Item {
	public:
		Crystal() {
			texID = TEXTURE.loadTexture("./assets/UI/crystal.png");
			_size = { 22.0f, 38.0f };
		}
		std::string getName() override {
			return "ŸŒ³…»";
		}
		ItemTag getTag() override {
			return ItemTag::CRYSTAL;
		}
};