#pragma once

#include "GameState.h"
#include "Camera.h"
#include "Field.h"
#include "FieldManager.h"
#include "StatusUI.h"
#include "IGameEvent.h"
#include "IGameEventHandler.h"
#include "LayerScreen.h"
#include "PostProcess.h"
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
		void updatePlayerAct() override;
		void moveCamera(Float3 targetPos) override;
		Float3& getCameraPos() override;
		void cameraVibration(bool isSet) override;
		RenderTexture* getScreenTex() override;
		void setGameEnd() override;
		void setCheckpoint(CheckPoint checkPoint) override;
		CheckPoint getCheckpoint() override;

	private:
		Camera* camera;
		Field* currentField;
		FieldManager* fieldManager;
		StatusUI statusUI;
		LayerScreen layerScreen;
		PostProcess* postProcess;
		RenderTexture* offscreenTex;
		CheckPoint currentCheckpoint = CheckPoint::JUMP;
		std::vector<IGameEvent*> gameEventQueue;
		bool isTriggerEntryField = false;
};