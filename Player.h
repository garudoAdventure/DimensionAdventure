#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "PlayerState.h"
#include "PlayerController.h"
#include "IGameEventHandler.h"
#include "Model.h"
#include "Spirit.h"

class PlayerState;

class Player : public GameObj {
  public:
		static constexpr float JUMP_FORCE = 0.75f;
		static constexpr float GRAVITY = 0.03f;

		void update() override;
		void draw() override;
		void setState(PlayerState* state);
		void changeState();

		void hitObj(GameObj* obj, bool isStatic = true) override;
		void jump();
		void convertDimension();
		void getDimensionAbility();
		void addCrystalNum();
		
		Model* getModel() {
			return _model;
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
		Spirit* getSpirit() {
			return _spirit;
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
		void setLayer(int layerIdx) {
			_currentLayer = layerIdx;
			_isConvertLayer = false;
		}
		int getCurrentLayer() const {
			return _currentLayer;
		}
		int getCrystalNum() {
			return _crystalNum;
		}
		bool hasDimensionRing() {
			return _hasDimensionAbility;
		}
		void getRemoteControl() {
			_hasRemoteControl = true;
		}
		bool hasRemoteControl() {
			return _hasRemoteControl;
		}

  private:
		Spirit* _spirit;
		Model* _model;
		Float3 _dir = { 1.0f, 0.0f, 0.0f };
		PlayerState* _currentState = nullptr;
		PlayerState* _newState = nullptr;
		PlayerController* _playerController = nullptr;
		IGameEventHandler* _gameEvent = nullptr;
		bool _isJump = false;
		bool _is2D = true;
		int _currentLayer = 0;
		bool _isConvertLayer = false;
		bool _hasDimensionAbility = true;
		bool _hasRemoteControl = false;
		int _crystalNum = 3;
		int _keydownTime = 0;

		void convertLayer();

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
		~Player();
		static inline Player* s_instance;
};

#define PLAYER Player::GetInstance()