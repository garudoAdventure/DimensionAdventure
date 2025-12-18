#pragma once

#include "GameObj.h"
#include "Player.h"
#include "MathTool.h"

class ActivableGameObj : public GameObj {
	public:
		ActivableGameObj() {
			_buttonTex = TEXTURE.loadTexture("./assets/keyEnter.png");
		}
		virtual void update() override {
			_isTrigger = false;
		}
		virtual void draw() override {
		}
		virtual void collide(GameObj* gameObj, bool is2D) override {
			this->trigger(gameObj, is2D);
		}
		void drawHint(Float3 pos) {
			if (_isTrigger) {
				XMMATRIX world = XMMatrixIdentity();
				world *= SHADER.getInverseView();
				world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
				SHADER.setWorldMatrix(world);
				SPRITE.drawSpriteIn3D({ 64.0f, 32.0f }, _buttonTex);
			}
		}
		virtual void trigger(GameObj* obj, bool is2D) {
			Cube triggerCube = { _pos, _triggerSize };
			if (MathTool::checkCollision(obj->getBox(), triggerCube, is2D)) {
				_isTrigger = true;
				onTrigger(obj);
			}
		}
		virtual void onTrigger(GameObj* obj) = 0;

	protected:
		Float3 _triggerSize = { 0.0f, 0.0f, 0.0f };
		bool _isTrigger = false;

	private:
		unsigned int _buttonTex;
};