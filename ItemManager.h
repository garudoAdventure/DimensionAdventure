#pragma once

#include "ObjManager.h"
#include "Item.h"
#include "MathTool.h"
#include <vector>

class ItemManager : public ObjManager {
	public:
		ItemManager() = default;
		~ItemManager() {
			for (Item* item : items) {
				delete item;
			}
		}
		void update() override {
			for (Item* item : items) {
				item->update();
			}
		}
		void draw() override {
			for (Item* item : items) {
				item->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
			for (Item* item : items) {
				if (MathTool::checkCollision(player->getBox(), item->getBox(), is2D)) {
					
				}
			}
		}
		void add(Item* item) {
			items.emplace_back(item);
		}

	private:
		std::vector<Item*> items;
};