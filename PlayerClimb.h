#pragma once

#include "PlayerState.h"
#include "GameObj.h"

class PlayerClimb : public PlayerState {
	public:
		PlayerClimb(GameObj* climbableObj);
		void update() override;
		void draw() override;

	private:
		GameObj* _climbableObj;
};