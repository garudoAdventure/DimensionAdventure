#pragma once

#include "Item.h"
#include "Texture.h"

class Key : public Item {
	public:
		Key() {
			texID = TEXTURE.loadTexture("./assets/key.png");
			_size = { 64.0f, 64.0f };
		}
		std::string getName() override {
			return "ÉJÉM";
		}
		ItemTag getTag() override {
			return ItemTag::KEY;
		}
};