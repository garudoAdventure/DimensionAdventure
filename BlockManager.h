#pragma once

#include "ObjManager.h"
#include "Block.h"
#include "MathTool.h"
#include <vector>

class BlockManager : public ObjManager {
	public:
		BlockManager() = default;
		~BlockManager() {
			for (Block* block : blocks) {
				delete block;
			}
		}
		void update() override {
			for (Block* block : blocks) {
				block->update();
			}
		}
		void draw() override {
			for (Block* block : blocks) {
				block->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
			for (Block* block : blocks) {
				if (MathTool::checkCollision(player->getBox(), block->getBox(), is2D)) {
					player->collide(block);
				}
			}
		}
		void add(Block* block) {
			blocks.emplace_back(block);
		}

	private:
		std::vector<Block*> blocks;
};