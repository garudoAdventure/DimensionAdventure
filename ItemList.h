#pragma once

#include "Item.h"
#include "Texture.h"
#include "Sprite.h"
#include "Keyboard.h"
#include <vector>

#include "RedBird.h"
#include "BlueDragon.h"
#include "WhiteTiger.h"
#include "GreenTurtle.h"
#include "Key.h"

class ItemList {
	public:
		ItemList() {
			texID = TEXTURE.loadTexture("./assets/itemList.png");
			_items.emplace_back(new RedBird());
			_items.emplace_back(new BlueDragon());
			_items.emplace_back(new WhiteTiger());
			_items.emplace_back(new GreenTurtle());
			_items.emplace_back(new Key());
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
			SPRITE.drawSprite2D({ 104.0f - 640.0f, -300.0f }, { 147.0f, 102.0f }, texID);
			_items[currentItem]->draw2D({ 104.0f - 640.0f, -300.0f });
		}
		void addItem(Item* item) {
			_items.emplace_back(item);
		}
		void removeItem() {
			_items.erase(_items.begin() + currentItem);
			currentItem = 0;
		}
		Item* getCurrentItem() {
			return _items[currentItem];
		}

	private:
		int texID;
		int currentItem = 0;
		std::vector<Item*> _items;
};