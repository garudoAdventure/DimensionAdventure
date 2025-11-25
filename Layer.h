#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"
#include "MathTool.h"
#include "FloorBaseManager.h"
#include "ClimbableObjManager.h"
#include "EnemyManager.h"
#include "IStagePuzzle.h"

class Layer {
	public:
		Layer() {
			_floorBaseManager = new FloorBaseManager();
			_climbableObkManager = new ClimbableObjManager();
			_enemyManager = new EnemyManager();
		}
		~Layer() {
			delete _floorBaseManager;
			delete _climbableObkManager;
			delete _enemyManager;
		}
		void update() {
			_floorBaseManager->update();
			_enemyManager->update();

			for (GameObj* gameObj : _gameObjs) {
				gameObj->update();
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->update();
			}
		}
		void draw() {
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, bgTex);
			_floorBaseManager->draw();
			_climbableObkManager->draw();
			_enemyManager->draw();

			for (GameObj* gameObj : _gameObjs) {
				gameObj->draw();
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->draw();
			}
		}
		void collisionCheck(Player* player, bool is2D) {
			_climbableObkManager->collide(player, is2D);
			_enemyManager->collide(player, is2D);
			
			for (GameObj* gameObj : _gameObjs) {
				gameObj->collide(player, is2D);
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->collide(player, is2D);
			}
		}
		void setBg(const char* path) {
			bgTex = TEXTURE.loadTexture(path);
		}
		FloorBaseManager* getFloorBaseManager() const {
			return _floorBaseManager;
		}
		ClimbableObjManager* getClimbableObjManager() const {
			return _climbableObkManager;
		}
		EnemyManager* getEnemyManager() const {
			return _enemyManager;
		}
		void addStagePuzzle(IStagePuzzle* stagePuzzle) {
			_stagePuzzles.emplace_back(stagePuzzle);
		}
		void addGameObj(GameObj* gameObj) {
			_gameObjs.emplace_back(gameObj);
		}

	private:
		int bgTex = -1;
		FloorBaseManager* _floorBaseManager;
		ClimbableObjManager* _climbableObkManager;
		EnemyManager* _enemyManager;
		std::vector<IStagePuzzle*> _stagePuzzles;
		std::vector<GameObj*> _gameObjs;
};