#pragma once

#include "Player.h"
#include "MathStruct.h"
#include "Layer.h"
#include "IGameEventHandler.h"

class Field {
  public:
		Field(const char* file);
		virtual ~Field();
		void update(int currentLayer);
		void draw(int currentLayer);
		void collide(Player* player, bool is2D);

	protected:
		Layer* _layer[3];
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
};