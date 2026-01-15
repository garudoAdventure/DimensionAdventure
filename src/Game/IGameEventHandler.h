#pragma once

#include "RenderTexture.h"
#include "./GameEvent/IGameEvent.h"
#include "./Common/MathStruct.h"

enum class CheckPoint {
	JUMP,
	FIND_FIRST_CRYSTAL,
	RED_CRYSTAL,
	GREEN_HINT,
	GREEN_CRYSTAL,
	BLUE_HINT,
	BLUE_CRYSTAL,
	WHITE_LAYER,
	REMOTE_CONTROL,
	WHITE_DOOR,
	PASSWORD,
	FINAL
};

class IGameEventHandler {
	public:
		virtual void addEvent(IGameEvent* gameEvent) = 0;
		virtual void setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) = 0;
		virtual void transformDimension() = 0;
		virtual void transformLayer() = 0;
		virtual void updateField() = 0;
		virtual void moveCamera(Float3 targetPos) = 0;
		virtual Float3& getCameraPos() = 0;
		virtual void cameraVibration(bool isSet) = 0;
		virtual RenderTexture* getScreenTex() = 0;
		virtual void setGameEnd() = 0;
		virtual void setCheckpoint(CheckPoint checkPoint) = 0;
		virtual CheckPoint& getCheckpoint() = 0;
		virtual void setSavePointPos(Float3& pos) = 0;
		virtual Float3& getSavePointPos() = 0;
		virtual int getBgmId() = 0;
		virtual void drawGameScene(int layerIdx) = 0;
		virtual void drawOffscreen(int layerIdx) = 0;
};