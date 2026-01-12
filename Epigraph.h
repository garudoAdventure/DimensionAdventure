#pragma once

#include "ActivableGameObj.h"
#include "Model.h"
#include "ModelManager.h"
#include "ShowDialogEvent.h"
#include "MessageDialog.h"
#include "IGameEventHandler.h"

class Epigraph : public ActivableGameObj {
	public:
		Epigraph(IGameEventHandler* gameEvent, Float3 pos, Float4 color, std::wstring content) : _gameEvent(gameEvent), _content(content) {
			_model = MODEL.loadModel("./assets/model/epigraph.fbx");
			_pos = pos;
			_size = _model->getSize();
			_triggerSize = { _size.x + 0.5f, _size.y + 0.5f, _size.z + 0.5f };
			_color = color;
		}
		void draw() override {
			_model->updateColor(_color);
			_model->draw(_pos, {0.0f, 0.0f, 0.0f});
		}
		void drawBillboard() {
			ActivableGameObj::drawHint({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
		}
		void onTrigger(GameObj* player) override {
			if (MathTool::checkCollision(player->getBox(), this->getBox(), false)) {
				player->hitObj(this);
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({{ Talker::SELF, _content }})
				));
			}
		}
	private:
		IGameEventHandler* _gameEvent;
		Model* _model;
		std::wstring _content;
};