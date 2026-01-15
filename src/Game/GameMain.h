#pragma once

#include "GameState.h"
#include "Camera.h"
#include "MazeBg.h"
#include "StatusUI.h"
#include "LayerScreen.h"
#include "PostProcess.h"
#include "./Field/Field.h"
#include "./Field/FieldManager.h"
#include "./GameEvent/IGameEvent.h"
#include "./Game/IGameEventHandler.h"
#include <vector>

class GameMain : public GameState, public IGameEventHandler {
	public:
		GameMain();
		~GameMain();
		void update() override;
		void draw() override;
		void addEvent(IGameEvent* gameEvent) override;
		void setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) override;
		void transformDimension() override;
		void transformLayer() override;
		void updateField() override;
		void moveCamera(Float3 targetPos) override;
		Float3& getCameraPos() override;
		void cameraVibration(bool isSet) override;
		RenderTexture* getScreenTex() override;
		void setGameEnd() override;
		void setCheckpoint(CheckPoint checkPoint) override;
		CheckPoint& getCheckpoint() override;
		void setSavePointPos(Float3& pos) override;
		Float3& getSavePointPos() override;
		int getBgmId() override;
		void drawGameScene(int layerIdx) override;
		void drawOffscreen(int layerIdx) override;

	private:
		unsigned int bgm;
		Camera* camera;
		Field* currentField;
		FieldManager* fieldManager;
		StatusUI statusUI;
		LayerScreen* layerScreen;
		PostProcess* postProcess;
		RenderTexture* offscreenTex;
		CheckPoint currentCheckpoint = CheckPoint::JUMP;
		std::vector<IGameEvent*> gameEventQueue;
		Float3 savePointPos{ 0.0f, 0.0f, 0.0f };
		bool isTriggerEntryField = false;
		MazeBg* mazeBg;
};