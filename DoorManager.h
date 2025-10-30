#pragma once

#include "ObjManager.h"
#include "Door.h"
#include "MathTool.h"
#include <vector>

class DoorManager : public ObjManager {
	public:
		DoorManager() = default;
		~DoorManager() {
			for (Door* door : doors) {
				delete door;
			}
		}
		void update() override {
			for (Door* door : doors) {
				door->update();
			}
		}
		void draw() override {
			for (Door* door : doors) {
				door->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
			for (Door* door : doors) {
				if (MathTool::checkCollision(player->getBox(), door->getBox(), is2D)) {
					door->entry();
				}
			}
		}
		void add(Door* door) {
			doors.emplace_back(door);
		}

	private:
		std::vector<Door*> doors;
};