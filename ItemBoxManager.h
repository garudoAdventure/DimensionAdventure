#pragma once

#include "ObjManager.h"
#include "ItemBox.h"
#include "MathTool.h"
#include <vector>

class ItemBoxManager : public ObjManager {
	public:
		ItemBoxManager() = default;
		~ItemBoxManager() {
			for (ItemBox* itemBox : itemBoxes) {
				delete itemBox;
			}
		}
		void update() override {
			for (ItemBox* itemBox : itemBoxes) {
				itemBox->update();
			}
		}
		void draw() override {
			for (ItemBox* itemBox : itemBoxes) {
				itemBox->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
			for (ItemBox* itemBox : itemBoxes) {
				itemBox->trigger(player, is2D);
			}
		}
		void add(ItemBox* itemBox) {
			itemBoxes.emplace_back(itemBox);
		}

	private:
		std::vector<ItemBox*> itemBoxes;
};