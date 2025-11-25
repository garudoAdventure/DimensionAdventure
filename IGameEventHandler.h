#pragma once

#include "MathStruct.h"
#include "IGameEvent.h"
#include "ItemList.h"

class IGameEventHandler {
	public:
		virtual void addEvent(IGameEvent* gameEvent) = 0;
		virtual void setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) = 0;
		virtual void transformDimension() = 0;
		virtual void transformLayer() = 0;
		virtual Float3& getCameraPos() = 0;
		virtual ItemList* getItemList() = 0;
		virtual void setFourGodCorrect(int idx, bool correct) = 0;
};