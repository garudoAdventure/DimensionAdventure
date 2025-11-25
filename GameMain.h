#pragma once

#include "GameState.h"
#include "Camera.h"
#include "Player.h"
#include "Field.h"
#include "StatusUI.h"
#include "ItemList.h"
#include "FieldManager.h"
#include "IGameEvent.h"
#include "IGameEventHandler.h"
#include "LayerScreen.h"
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
		Float3& getCameraPos() override;
		ItemList* getItemList() override;
		void setFourGodCorrect(int idx, bool correct) override;

	private:
		Camera* camera;
		Player* player;
		Float3 _playerInitPos = { 0.0f, 0.0f, 0.0f };
		Field* newField;
		Field* currentField;
		FieldManager* fieldManager;
		StatusUI statusUI;
		ItemList itemList;
		LayerScreen layerScreen;
		std::vector<IGameEvent*> gameEventQueue;
		bool fourGodCorrect[4] = { false, false, false, false };

		void changeField();
};