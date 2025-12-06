#pragma once

#include "GameState.h"
#include "Camera.h"
#include "Field.h"
#include "StatusUI.h"
#include "ItemList.h"
#include "FieldManager.h"
#include "IGameEvent.h"
#include "IGameEventHandler.h"
#include "LayerScreen.h"
#include <vector>
#include "PostProcess.h"

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
		Float3& getCameraPos() override;
		virtual void setCameraVibration(bool isSet) override;

	private:
		Camera* camera;
		Field* currentField;
		FieldManager* fieldManager;
		StatusUI statusUI;
		ItemList itemList;
		LayerScreen layerScreen;
		std::vector<IGameEvent*> gameEventQueue;
		PostProcess* postProcess;
		RenderTexture* offscreenTex;
};