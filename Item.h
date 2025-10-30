#pragma once

#include "GameObj.h"
#include "Player.h"
#include "Texture.h"
#include "Sprite.h"
#include <string>

class Item : public GameObj {
	public:
		Item(Float3 pos, Float3 size) {
			_pos = pos;
			_size = size * 2;
			_color = { 1.0f, 0.5f, 0.2f, 0.4f };
		}
		virtual void update() {}
		void draw() override {}
		virtual void draw(Float3 pos) {}
		virtual void get(Player* player) = 0;
		virtual std::string getName() = 0;
};

class Crystal : public Item {
	public:
		Crystal(Float3 pos, Float3 size) : Item(pos, size) {
			texID = TEXTURE.loadTexture("./assets/UI/crystal.png");
		}
		void draw(Float3 pos) override {
			SPRITE.drawSprite3D(pos, { 22.0f, 38.0f }, texID);
		}
		void get(Player* player) override {
			player->addCrystal();
		}
		std::string getName() override {
			return "ŸŒ³…»";
		}

	private:
		unsigned int texID;
};