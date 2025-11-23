#pragma once

#include "ObjManager.h"
#include "FloorBase.h"
#include "MathTool.h"
#include <vector>

class FloorBaseManager : public ObjManager {
	public:
		FloorBaseManager() = default;
		~FloorBaseManager() {
			for (FloorBase* floorBase : floorBases) {
				delete floorBase;
			}
		}
		void update() override {
			for (FloorBase* floorBase : floorBases) {
				floorBase->update();
			}
		}
		void draw() override {
			for (FloorBase* floorBase : floorBases) {
				floorBase->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
		}
		void add(FloorBase* block) {
			floorBases.emplace_back(block);
		}

	private:
		std::vector<FloorBase*> floorBases;
};