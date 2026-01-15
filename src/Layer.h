#pragma once

#include "./StagePuzzle/IStagePuzzle.h"
#include "./Player/Player.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Utils/MathTool.h"

enum LayerType {
	WHITE,
	RED,
	GREEN,
	BLUE
};

class Layer {
	public:
		Layer() {
			_gameObjs.reserve(500);
			_stagePuzzles.reserve(3);
		}
		~Layer() {
			for (GameObj* obj : _gameObjs) {
				delete obj;
			}
			for (IStagePuzzle* puzzle : _stagePuzzles) {
				delete puzzle;
			}
		}
		void update() {
			for (GameObj* gameObj : _gameObjs) {
				if (gameObj->isActive()) {
					gameObj->update();
				}
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->update();
			}
		}
		void draw() {
			for (GameObj* gameObj : _gameObjs) {
				if (gameObj->isActive()) {
					gameObj->draw();
				}
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->draw();
			}
		}
		void drawBillboard() {
			for (GameObj* gameObj : _gameObjs) {
				if (gameObj->isActive()) {
					gameObj->drawBillboard();
				}
			}
		}
		void collisionCheck(bool is2D) {
			for (GameObj* gameObj : _gameObjs) {
				if (!gameObj->isActive()) continue;
				gameObj->collide(&PLAYER, is2D);
			}
			for (IStagePuzzle* stagePuzzle : _stagePuzzles) {
				stagePuzzle->collide(&PLAYER, is2D);
			}
		}
		void addGameObj(GameObj* gameObj) {
			_gameObjs.emplace_back(gameObj);
		}
		void addStagePuzzle(IStagePuzzle* stagePuzzle) {
			_stagePuzzles.emplace_back(stagePuzzle);
		}

	private:
		std::vector<GameObj*> _gameObjs;
		std::vector<IStagePuzzle*> _stagePuzzles;
};