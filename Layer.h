#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "MathTool.h"
#include "IStagePuzzle.h"
#include "Shader.h"

class Layer {
	public:
		Layer() = default;
		~Layer() {
			for (GameObj* obj : _gameObjs) {
				delete obj;
			}
		}
		void update() {
			for (GameObj* gameObj : _gameObjs) {
				if (gameObj->isActive()) {
					gameObj->update();
				}
			}
			for (Enemy* enemy : _enemies) {
				enemy->update();
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->update();
			}
		}
		void draw() {
			// SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, bgTex);
			for (GameObj* gameObj : _gameObjs) {
				if (gameObj->isActive()) {
					gameObj->draw();
				}
			}
			for (Enemy* enemy : _enemies) {
				enemy->draw();
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->draw();
			}
		}
		void collisionCheck(bool is2D) {
			for (GameObj* gameObj : _gameObjs) {
				if (!gameObj->isActive()) continue;
				gameObj->collide(&PLAYER, is2D);
			}
			//for (Enemy* enemy : _enemies) {
			//	enemy->collide(&PLAYER, is2D);
			//}
			//for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
			//	stagePuzzle->collide(&PLAYER, is2D);
			//}
		}
		void setBg(const char* path) {
			bgTex = TEXTURE.loadTexture(path);
		}
		void addGameObj(GameObj* gameObj) {
			_gameObjs.emplace_back(gameObj);
		}
		void addEnemy(Enemy* enemy) {
			_enemies.emplace_back(enemy);
		}
		void addStagePuzzle(IStagePuzzle* stagePuzzle) {
			_stagePuzzles.emplace_back(stagePuzzle);
		}

	private:
		int bgTex = -1;
		std::vector<IStagePuzzle*> _stagePuzzles;
		std::vector<GameObj*> _gameObjs;
		std::vector<Enemy*> _enemies;
};