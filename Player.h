#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "PlayerState.h"
#include "PlayerController.h"
#include "IGameEventHandler.h"
#include "Enemy.h"
#include "Model.h"
#include "Spirit.h"

#define MOVE_VEL		(0.2f)
#define JUMP_FORCE  (0.7f)
#define GRAVITY			(0.03f)

class PlayerState;

class Player : public GameObj {
  public:
		void update() override;
		void draw() override;
		void setState(PlayerState* state);

		void hitObj(GameObj* obj, bool isStatic = true) override;
		void jump();
		void climb(GameObj* climableObj);
		void hurt(int damage);
		void attackEnemy(Enemy* enemy);
		void convertDimension();
		void getDimensionAbility();
		void addCrystalNum();
		void setToEventState(bool isEventState);
		
		Model* getModel() {
			return _model;
		}
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
		int getCrystalNum() {
			return _crystalNum;
		}
		Spirit* getSpirit() {
			return _spirit;
		}

  private:
		Spirit* _spirit;
		Model* _model;
		Float3 _dir = { 1.0f, 0.0f, 0.0f };
		PlayerState* currentState = nullptr;
		PlayerState* newState = nullptr;
		PlayerController* _playerController = nullptr;
		IGameEventHandler* _gameEvent = nullptr;
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
		bool _hasDimensionAbility = true;
		int _crystalNum = 1;

		void changeState();
		void convertLayer();
		void autoRecoverEnergy();
		void invincibleUpdate();
		void attackModeUpdate();

	public:
		static void CreateInstance(IGameEventHandler* gameEvent) {
			DeleteInstance();
			s_instance = new Player(gameEvent);
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static Player& GetInstance() {
			return *s_instance;
		}

	private:
		Player(IGameEventHandler* gameEvent);
		~Player() {}
		static inline Player* s_instance;
};

#define PLAYER Player::GetInstance()