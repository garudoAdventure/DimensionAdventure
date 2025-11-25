#pragma once

#include "GameObj.h"
#include "Dialog.h"
#include "Player.h"
#include "MathTool.h"

class ActivableGameObj : public GameObj {
	public:
		ActivableGameObj() {
			_hintDialog = new HintDialog();
		}
		virtual void update() override {
			_isTrigger = false;
		}
		virtual void draw() override {
			if (_isTrigger) {
				_hintDialog->draw({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
			}
		}
		virtual void collide(GameObj* gameObj, bool is2D) override {
			this->trigger(gameObj, is2D);
		}
		void trigger(GameObj* player, bool is2D) {
			Cube triggerCube = { _pos, _triggerSize };
			if (MathTool::checkCollision(player->getBox(), triggerCube, is2D)) {
				_isTrigger = true;
				onTrigger(player);
			}
		}
		virtual void onTrigger(GameObj* player) = 0;

	protected:
		Float3 _triggerSize = { 0.0f, 0.0f, 0.0f };

	private:
		bool _isTrigger = false;
		IDialog* _hintDialog = nullptr;
};