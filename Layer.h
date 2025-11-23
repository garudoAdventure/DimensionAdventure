#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"
#include "MathTool.h"
#include "BlockManager.h"
#include "FloorBaseManager.h"
#include "DoorManager.h"
#include "ClimbableObjManager.h"
#include "EnemyManager.h"
#include "ItemBoxManager.h"
#include "IStagePuzzle.h"

class Layer {
	public:
		Layer() {
			_blockManager = new BlockManager();
			_floorBaseManager = new FloorBaseManager();
			_doorManager = new DoorManager();
			_climbableObkManager = new ClimbableObjManager();
			_enemyManager = new EnemyManager();
			_itemBoxManager = new ItemBoxManager();
		}
		~Layer() {
			delete _blockManager;
			delete _floorBaseManager;
			delete _doorManager;
			delete _climbableObkManager;
			delete _enemyManager;
			delete _itemBoxManager;
		}
		void update() {
			_blockManager->update();
			_floorBaseManager->update();
			_doorManager->update();
			_enemyManager->update();
			_itemBoxManager->update();
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->update();
			}
		}
		void draw() {
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, bgTex);
			_blockManager->draw();
			_floorBaseManager->draw();
			_doorManager->draw();
			_climbableObkManager->draw();
			_enemyManager->draw();
			_itemBoxManager->draw();
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->draw();
			}
		}
		void collide(Player* player, bool is2D) {
			_blockManager->collide(player, is2D);
			_doorManager->collide(player, is2D);
			_climbableObkManager->collide(player, is2D);
			_enemyManager->collide(player, is2D);
			_itemBoxManager->collide(player, is2D);
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->collide(player, is2D);
			}
		}
		void setBg(const char* path) {
			bgTex = TEXTURE.loadTexture(path);
		}
		BlockManager* getBlockManager() const {
			return _blockManager;
		}
		FloorBaseManager* getFloorBaseManager() const {
			return _floorBaseManager;
		}
		DoorManager* getDoorManager() const {
			return _doorManager;
		}
		ClimbableObjManager* getClimbableObjManager() const {
			return _climbableObkManager;
		}
		EnemyManager* getEnemyManager() const {
			return _enemyManager;
		}
		ItemBoxManager* getItemBoxManager() const {
			return _itemBoxManager;
		}
		void addStagePuzzle(IStagePuzzle* stagePuzzle) {
			_stagePuzzles.emplace_back(stagePuzzle);
		}

	private:
		int bgTex = -1;
		BlockManager* _blockManager;
		FloorBaseManager* _floorBaseManager;
		DoorManager* _doorManager;
		ClimbableObjManager* _climbableObkManager;
		EnemyManager* _enemyManager;
		ItemBoxManager* _itemBoxManager;
		std::vector<IStagePuzzle*> _stagePuzzles;
};