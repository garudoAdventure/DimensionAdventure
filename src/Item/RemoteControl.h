#pragma once

#include "Item.h"
#include "./GameEvent/GetItemEvent.h"
#include "./Player/Player.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Common/Color.h"

class RemoteControl : public Item {
	public:
		RemoteControl(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
			_tex = TEXTURE.loadTexture("./assets/remoteControl.png");
		}

		void drawBillboard() override {
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
			SHADER.setWorld(world);
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 63.0f, 60.0f }, TEXTURE.getTexture(_tex), Color::white);
		}

		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(_gameEvent, this, [=]() {
				PLAYER.getRemoteControl();
				_gameEvent->setCheckpoint(CheckPoint::REMOTE_CONTROL);
			}));
		}

		std::wstring getName() override {
			return L"出口の制御装置";
		}

		ItemTag getTag() override {
			return ItemTag::REMOTE_CONTROL;
		}

	private:
		unsigned int _tex;
};