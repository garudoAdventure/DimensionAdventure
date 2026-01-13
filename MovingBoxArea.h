#pragma once

#include "Block.h"
#include "Wall.h"
#include "MathTool.h"
#include "ModelManager.h"
#include "IStagePuzzle.h"
#include "IGameEventHandler.h"
#include "SystemDialog.h"
#include "ShowDialogEvent.h"
#include "Color.h"
#include <functional>

class MovingBoxArea : public IStagePuzzle {
	public:
		static constexpr int BOX_NUM = 4;

		MovingBoxArea(IGameEventHandler* gameEvent, std::function<void()> callback) : _gameEvent(gameEvent), _callback(callback) {
			_boxGrid = MODEL.loadModel("./assets/model/boxGrid.fbx");
			Model* boxModel[BOX_NUM] = {
				MODEL.loadModel("./assets/model/boxNum4.fbx"),
				MODEL.loadModel("./assets/model/boxNum1.fbx"),
				MODEL.loadModel("./assets/model/boxNum3.fbx"),
				MODEL.loadModel("./assets/model/boxNum2.fbx"),
			};
			const Int2 boxCoord[BOX_NUM] = {
				{ 3, 0 }, { 2, 1 }, { 1, 2 }, { 0, 3 }
			};
			for (int i = 0; i < BOX_NUM; i++) {
				_box[i] = new MovableBox(MathTool::getCoordPos({
					_startX - boxCoord[i].x * 2, _startY, _startZ + boxCoord[i].y * 2
				}), { 1.0f, 1.0f, 1.0f }, Color::white, boxModel[i]);
			}
			_gridBorder[0] = new Wall(
				MathTool::getCoordPos({ _startX - 3.0f, _startY, _startZ - 1.0f }),
				{ 24.0f, 3.0f, 3.0f }
			);
			_gridBorder[1] = new Wall(
				MathTool::getCoordPos({ _startX - 3.0f, _startY, _startZ + 8.0f }),
				{ 24.0f, 3.0f, 3.0f }
			);
			_gridBorder[2] = new Wall(
				MathTool::getCoordPos({ _startX + 1.5f, _startY, _startZ + 3.5f }),
				{ 3.0f, 3.0f, 24.0f }
			);
			_gridBorder[3] = new Wall(
				MathTool::getCoordPos({ _startX - 7.5f, _startY, _startZ + 3.5f }),
				{ 3.0f, 3.0f, 24.0f }
			);
		}

		~MovingBoxArea() {
			for (int i = 0; i < BOX_NUM; i++) {
				delete _box[i];
			}
			delete _gridBorder[0];
			delete _gridBorder[1];
			delete _gridBorder[2];
			delete _gridBorder[3];
		}

		void update() override {			
			for (int i = 0; i < BOX_NUM; i++) {
				_box[i]->update();
			}
			if (!_isSolved && checkAnswer()) {
				_isSolved = true;
				_gameEvent->addEvent(new ShowDialogEvent(
					new SystemDialog({ L"奥になにが現れた！" })
				));
				_callback();
			}
		}

		void draw() override {
			_boxGrid->updateColor(_isSolved ? Color::lightGreen : Color::yellow);
			_boxGrid->draw(MathTool::getCoordPos({
				_startX - 3.0f, 0.0f, _startZ + 3.5f
			}), { 0.0f, 0.0f, 0.0f });
			for (int i = 0; i < BOX_NUM; i++) {
				_box[i]->draw();
			}
		}

		void collide(Player* player, bool is2D) override {
			for (int i = 0; i < BOX_NUM; i++) {
				if (MathTool::checkCollision(player->getBox(), _box[i]->getBox(), is2D)) {
					// Player Hit Block, update block position
					player->hitObj(_box[i], _isSolved);

					// Check collision with other box
					for (int j = 0; j < BOX_NUM; j++) {
						if (i == j) continue;
						if (MathTool::checkCollision(_box[i]->getBox(), _box[j]->getBox(), is2D)) {
							_box[i]->hitObj(_box[j]);
						}
					}

					// Hit border
					for (int j = 0; j < 4; j++) {
						if (MathTool::checkCollision(_box[i]->getBox(), _gridBorder[j]->getBox(), is2D)) {
							_box[i]->hitObj(_gridBorder[j]);
						}
					}

					// Update player position
					player->hitObj(_box[i]);
				}
			}
		}

		bool checkAnswer() {
			bool box1Correct =
				abs(_answerPos[3].x - _box[1]->getPos().x) < 1.6f &&
				abs(_answerPos[3].z - _box[1]->getPos().z) < 1.6f;

			bool box2Correct =
				abs(_answerPos[2].x - _box[3]->getPos().x) < 1.6f &&
				abs(_answerPos[2].z - _box[3]->getPos().z) < 1.6f;

			bool box3Correct =
				abs(_answerPos[1].x - _box[2]->getPos().x) < 1.6f &&
				abs(_answerPos[1].z - _box[2]->getPos().z) < 1.6f;

			bool box4Correct =
				abs(_answerPos[0].x - _box[0]->getPos().x) < 1.6f &&
				abs(_answerPos[0].z - _box[0]->getPos().z) < 1.6f;

			if (box1Correct && box2Correct && box3Correct && box4Correct) {
				return true;
			}
			return false;
		}

	private:
		IGameEventHandler* _gameEvent;
		std::function<void()> _callback;
		Model* _boxGrid;
		Wall* _gridBorder[4];
		MovableBox* _box[BOX_NUM];
		bool _isSolved = false;
		const float _startX = 24.5f;
		const float _startY = 1.0f;
		const float _startZ = 2.0f;
		const Float3 _answerPos[4] = {
			MathTool::getCoordPos({ _startX - 1 * 2.0f, _startY, _startZ + 0.5f + 0 * 2.0f }),
			MathTool::getCoordPos({ _startX - 3 * 2.0f, _startY, _startZ + 0.5f + 1 * 2.0f }),
			MathTool::getCoordPos({ _startX - 0 * 2.0f, _startY, _startZ + 0.5f + 2 * 2.0f }),
			MathTool::getCoordPos({ _startX - 2 * 2.0f, _startY, _startZ + 0.5f + 3 * 2.0f }),
		};

		// Answer
		// X Z	  3			2			 1			0
		// 0 | { 0,     (2-4), 0,     0     }
		// 1 | { 0,     0,     0,     (4-3) }  
		// 2 | { (1-2), 0,     0,     0     }  ↑
		// 3 | { 0,     0,     (3-1), 0     }  
		//                →
};