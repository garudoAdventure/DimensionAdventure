#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "PlayerState.h"
#include "PlayerController.h"
#include "IGameEventHandler.h"
#include "Enemy.h"

#define MOVE_VEL		(0.2f)
#define JUMP_FORCE  (0.7f)
#define GRAVITY			(0.03f)

class PlayerState;

class Player : public GameObj {
  public:
		Player(IGameEventHandler* gameEvent);
		void update();
		void draw();
		void setState(PlayerState* state);

		void collide(GameObj* gameObj);
		void jump();
		void climb(GameObj* climableObj);
		void hurt(int damage);
		void attackEnemy(Enemy* enemy);
		void convertDimension();

		int getHp() const {
			return _hp;
		}
		int getEnergy() const {
			return _energy;
		}
		void setPos(Float3 pos) {
			_pos = pos;
		}
		Float3& getVel() {
			return _vel;
		}
		Float3 getDir() const {
			return _dir;
		}
		PlayerController* getPlayerController() const {
			return _playerController;
		}
		void setJump(bool isJump) {
			_isJump = isJump;
		}
		bool isJump() const {
			return _isJump;
		}
		bool is2D() const {
			return _is2D;
		}
		void setIsClimbing(bool isClimbing) {
			_isOnClimbableObj = isClimbing;
		}
		void setLayer(int layerIdx) {
			_currentLayer = layerIdx;
			_isConvertLayer = false;
		}
		int getCurrentLayer() const {
			return _currentLayer;
		}
		void setAttackMode(bool isAttackMode) {
			_isAttackMode = isAttackMode;
		}
		void getBangle() {
			hasBangle = true;
		}
		void addCrystal() {
			_crystalNum += 1;
		}
		int getCrystalNum() {
			return _crystalNum;
		}

  private:
		Float3 _oldPos = { 0.2f, 0.8f, 0.0f };
		Float3 _vel = { 0.0f, 0.0f, 0.0f };
		Float3 _dir = { 1.0f, 0.0f, 0.0f };
		PlayerState* currentState = nullptr;
		PlayerState* newState = nullptr;
		PlayerController* _playerController = nullptr;
		IGameEventHandler* _gameEvent;
		int _hp = 5;
		int _energy = 5;
		int _recoverEnergyCount = 0;
		bool _isJump = false;
		bool _is2D = true;
		int _currentLayer = 0;
		bool _isConvertLayer = false;
		bool _isOnClimbableObj = false;
		bool _isInvincible = false;
		bool _isAttackMode = false;
		bool hasBangle = true;
		int _crystalNum = 2;

		void changeState();
		void convertLayer();
		void autoRecoverEnergy();
		void invincibleUpdate();
		void attackModeUpdate();
};