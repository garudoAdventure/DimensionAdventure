#pragma once

#include "Item.h"
#include "./GameEvent/GetItemEvent.h"
#include "./GameEvent/ShowDialogEvent.h"
#include "./Dialog/DimensionTutorialDialog.h"
#include "./Player/Player.h"
#include "./Render/Texture.h"

class DimensionRing : public Item {
	public:
		DimensionRing(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
			_tex = TEXTURE.loadTexture("./assets/UI/circleArrow.png");
		}
		void drawBillboard() override {
			_rotate -= 0.005f;
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixRotationZ(_rotate);
			world *= XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
			SHADER.setWorld(world);
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 63.0f, 60.0f }, TEXTURE.getTexture(_tex), Color::white);
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(_gameEvent, this, [=]() {
				PLAYER.getDimensionAbility();
				_gameEvent->addEvent(new ShowDialogEvent(
					new DimensionTutorialDialog()
				));
			}));
		}
		std::wstring getName() override {
			return L"次元リングをゲット！";
		}
		ItemTag getTag() override {
			return ItemTag::RING;
		}

	private:
		unsigned int _tex;
		float _rotate = 0.0f;
};