#pragma once

#include "./Common/MathStruct.h"

enum class ObjTag {
	NONE,
	PLAYER_TAG,
	ITEM,
	LADDER,
	MOVING_FLOOR,
	PLAYER_FALL_POINT
};

class GameObj {
	public:
		virtual ~GameObj() = default;
		virtual void update();
		virtual void draw();
		virtual void drawBillboard() {}
		virtual void collide(GameObj* obj, bool is2D);
		virtual void hitObj(GameObj* obj, bool isStatic = true);
		void setPos(Float3 pos);
		void setPosX(float posX);
		void setPosY(float posY);
		void setPosZ(float posZ);
		void setColor(Float4 color);
		Float3 getPos() const;
		Float3 getVel() const;
		Float3 getSize() const;
		Cube getBox() const;
		ObjTag getTag() const;
		bool isActive() const;

	protected:
		Float3 _pos;
		Float3 _oldPos;
		Float3 _vel = { 0.0f, 0.0f, 0.0f };
		Float3 _size;
		Float4 _color = { 0.3f, 0.3f, 0.3f, 0.7f };
		int _texID = -1;
		ObjTag _tag = ObjTag::NONE;
		bool _isActive = true;
};