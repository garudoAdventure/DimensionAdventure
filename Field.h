#pragma once

#include "MathStruct.h"
#include "Layer.h"
#include "IGameEventHandler.h"
#include "Color.h"
#include "CameraFocusEvent.h"
#include "Player.h"
#include "PlayerFreeze.h"

class Field {
  public:
		Field(const char* file);
		virtual ~Field();
		void update(int currentLayer);
		void draw(int currentLayer);
		void load();
		void collisionCheck(bool is2D);
		virtual void onEntryField() { }

	protected:
		const char* filePath;
		Layer* _layer[4];
		Float4 layerColor[4] = {
			Color::white,
			Color::lightRed,
			Color::lightGreen,
			Color::lightBlue,
		};
};

class Field00 : public Field {
	public:
		Field00(IGameEventHandler* gameEvent);
};

class Field01 : public Field {
	public:
		Field01(IGameEventHandler* gameEvent);
};

class Field02 : public Field {
	public:
		Field02(IGameEventHandler* gameEvent);
};

class Field03 : public Field {
	public:
		Field03(IGameEventHandler* gameEvent);
};

class Field04 : public Field {
	public:
		Field04(IGameEventHandler* gameEvent);
		void onEntryField() override {
			if (!isLookRedCrystal) {
				isLookRedCrystal = true;
				PLAYER.setState(new PlayerFreeze());
				_gameEvent->addEvent(new CameraFocusEvent(_gameEvent,
					MathTool::getCoordPos({ 39.0f, 3.5f, 5.0f })
				));
			}
		}

	private:
		IGameEventHandler* _gameEvent;
		bool isLookRedCrystal = false;
};

class Field05 : public Field {
	public:
		Field05(IGameEventHandler* gameEvent);
};

class Field51 : public Field {
	public:
		Field51(IGameEventHandler* gameEvent);
};

class Field52 : public Field {
	public:
		Field52(IGameEventHandler* gameEvent);
};

class Field53 : public Field {
	public:
		Field53(IGameEventHandler* gameEvent);
};

class Field54 : public Field {
	public:
		Field54(IGameEventHandler* gameEvent);
		void onEntryField() override {
			if (!isLookGreenCrystal) {
				isLookGreenCrystal = true;
				PLAYER.setState(new PlayerFreeze());
				_gameEvent->addEvent(new CameraFocusEvent(_gameEvent,
					MathTool::getCoordPos({ 39.0f, 4.5f, 5.0f })
				));
			}
		}

	private:
		IGameEventHandler* _gameEvent;
		bool isLookGreenCrystal = false;
};

class Field06 : public Field {
	public:
		Field06(IGameEventHandler* gameEvent);
};