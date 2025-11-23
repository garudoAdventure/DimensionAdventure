#pragma once

#include "Item.h"
#include "Texture.h"
#include "Sprite.h"
#include "Keyboard.h"
#include <vector>

#include "RedBird.h"
#include "BlueDragon.h"

class ItemList {
	public:
		ItemList() {
			texID = TEXTURE.loadTexture("./assets/itemList.png");
			_items.emplace_back(new RedBird());
			_items.emplace_back(new BlueDragon());
		}
		void update() {
			if (Keyboard_IsKeyTrigger(KK_A)) {
				currentItem = currentItem - 1 < 0 ? _items.size() - 1 : currentItem - 1;
			}
			if (Keyboard_IsKeyTrigger(KK_S)) {
				currentItem = (currentItem + 1) % _items.size();
			}
		}
		void draw() {
			SPRITE.drawSprite2D({ 104.0f - 640.0f, -650.0f + 360.0f }, { 147.0f, 102.0f }, texID);
			_items[currentItem]->draw2D({ 104.0f - 640.0f, -650.0f + 360.0f });
		}
		void addItem(Item* item) {
			_items.emplace_back(item);
		}

	private:
		int texID;
		int currentItem = 0;
		std::vector<Item*> _items;
};