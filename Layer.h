#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"
#include "MathTool.h"
#include "BlockManager.h"
#include "DoorManager.h"
#include "ClimbableObjManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "ItemBoxManager.h"

class Layer {
	public:
		Layer(const char* path) {
			bgTex = TEXTURE.loadTexture(path);
			_blockManager = new BlockManager();
			_doorManager = new DoorManager();
			_climbableObkManager = new ClimbableObjManager();
			_enemyManager = new EnemyManager();
			_itemManager = new ItemManager();
			_itemBoxManager = new ItemBoxManager();
		}
		~Layer() {
			delete _blockManager;
			delete _doorManager;
			delete _climbableObkManager;
			delete _enemyManager;
			delete _itemManager;
			delete _itemBoxManager;
		}
		void update() {
			_blockManager->update();
			_doorManager->update();
			_enemyManager->update();
			_itemManager->update();
			_itemBoxManager->update();
		}
		void draw() {
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, bgTex);
			_blockManager->draw();
			_doorManager->draw();
			_climbableObkManager->draw();
			_enemyManager->draw();
			_itemManager->draw();
			_itemBoxManager->draw();
		}
		void collide(Player* player, bool is2D) {
			_blockManager->collide(player, is2D);
			_doorManager->collide(player, is2D);
			_climbableObkManager->collide(player, is2D);
			_enemyManager->collide(player, is2D);
			_itemManager->collide(player, is2D);
			_itemBoxManager->collide(player, is2D);
		}
		BlockManager* getBlockManager() const {
			return _blockManager;
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
		ItemManager* getItemManager() const {
			return _itemManager;
		}
		ItemBoxManager* getItemBoxManager() const {
			return _itemBoxManager;
		}

	private:
		unsigned int bgTex;
		BlockManager* _blockManager;
		DoorManager* _doorManager;
		ClimbableObjManager* _climbableObkManager;
		EnemyManager* _enemyManager;
		ItemManager* _itemManager;
		ItemBoxManager* _itemBoxManager;
};