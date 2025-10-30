#pragma once

#include "ObjManager.h"
#include "Enemy.h"
#include "MathTool.h"
#include <vector>

class EnemyManager : public ObjManager {
	public:
		EnemyManager() = default;
		~EnemyManager() {
			for (Enemy* enemy : enemies) {
				delete enemy;
			}
		}
		void update() override {
			for (Enemy* enemy : enemies) {
				if (enemy->isDead()) {
					continue;
				}
				enemy->update();
			}
		}
		void draw() override {
			for (Enemy* enemy : enemies) {
				if (enemy->isDead()) {
					continue;
				}
				enemy->draw();
			}
		}
		void collide(Player* player, bool is2D) override {
			for (Enemy* enemy : enemies) {
				if (enemy->isDead()) {
					continue;
				}
				if (MathTool::checkCollision(player->getBox(), enemy->getBox(), is2D)) {
					if (MathTool::checkCollision(player->getBox(), enemy->getBox(), is2D)) {
						player->hurt(1);
						player->attackEnemy(enemy);
					}
				}
			}
		}
		void add(Enemy* enemy) {
			enemies.emplace_back(enemy);
		}

	private:
		std::vector<Enemy*> enemies;
};