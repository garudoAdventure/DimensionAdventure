#pragma once

#include "IStagePuzzle.h"
#include "./Object/Block.h"
#include "./Object/Wall.h"
#include "./Game/IGameEventHandler.h"
#include "./GameEvent/ShowDialogEvent.h"
#include "./Dialog/SystemDialog.h"
#include "./Render/ModelManager.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include <functional>
#include <array>

class MovingBoxArea : public IStagePuzzle {
	public:
		static constexpr int BOX_NUM = 4;

		MovingBoxArea(IGameEventHandler* gameEvent, std::function<void()> callback) : _gameEvent(gameEvent), _callback(callback) {
			_boxGrid = MODEL.loadModel("./assets/model/boxGrid.fbx");
			std::array<Model*, BOX_NUM> boxModel = {
				MODEL.loadModel("./assets/model/boxNum4.fbx"),
				MODEL.loadModel("./assets/model/boxNum1.fbx"),
				MODEL.loadModel("./assets/model/boxNum3.fbx"),
				MODEL.loadModel("./assets/model/boxNum2.fbx"),
			};
			const std::array<Int2, BOX_NUM> boxCoord = {
				Int2{ 3, 0 }, Int2{ 2, 1 }, Int2{ 1, 2 }, Int2{ 0, 3 }
			};
			for (int i = 0; i < BOX_NUM; i++) {
				_box.at(i) = new PushableBox(MathTool::getCoordPos({
					_startX - boxCoord.at(i).x * 2, _startY, _startZ + boxCoord.at(i).y * 2
				}), { 1.0f, 1.0f, 1.0f }, Color::white, boxModel.at(i));
			}
			_gridBorder.at(0) = new Wall(
				MathTool::getCoordPos({ _startX - 3.0f, _startY, _startZ - 1.0f }),
				{ 24.0f, 3.0f, 3.0f }
			);
			_gridBorder.at(1) = new Wall(
				MathTool::getCoordPos({ _startX - 3.0f, _startY, _startZ + 8.0f }),
				{ 24.0f, 3.0f, 3.0f }
			);
			_gridBorder.at(2) = new Wall(
				MathTool::getCoordPos({ _startX + 1.5f, _startY, _startZ + 3.5f }),
				{ 3.0f, 3.0f, 24.0f }
			);
			_gridBorder.at(3) = new Wall(
				MathTool::getCoordPos({ _startX - 7.5f, _startY, _startZ + 3.5f }),
				{ 3.0f, 3.0f, 24.0f }
			);
		}

		~MovingBoxArea() {
			for (int i = 0; i < BOX_NUM; i++) {
				delete _box.at(i);
			}
			delete _gridBorder.at(0);
			delete _gridBorder.at(1);
			delete _gridBorder.at(2);
			delete _gridBorder.at(3);
		}

		void update() override {			
			for (int i = 0; i < BOX_NUM; i++) {
				_box.at(i)->update();
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
				_box.at(i)->draw();
			}
		}

		void collide(Player* player, bool is2D) override {
			for (int i = 0; i < BOX_NUM; i++) {
				if (MathTool::checkCollision(player->getBox(), _box.at(i)->getBox(), is2D)) {
					// Player Hit Block, update block position
					player->hitObj(_box.at(i), _isSolved);

					// Check collision with other box
					for (int j = 0; j < BOX_NUM; j++) {
						if (i == j) continue;
						if (MathTool::checkCollision(_box.at(i)->getBox(), _box.at(j)->getBox(), is2D)) {
							_box.at(i)->hitObj(_box.at(j));
						}
					}

					// Hit border
					for (int j = 0; j < 4; j++) {
						if (MathTool::checkCollision(_box.at(i)->getBox(), _gridBorder.at(j)->getBox(), is2D)) {
							_box.at(i)->hitObj(_gridBorder.at(j));
						}
					}

					// Update player position
					player->hitObj(_box.at(i));
				}
			}
		}

		bool checkAnswer() {
			bool box1Correct =
				abs(_answerPos.at(3).x - _box.at(1)->getPos().x) < 1.6f &&
				abs(_answerPos.at(3).z - _box.at(1)->getPos().z) < 1.6f;

			bool box2Correct =
				abs(_answerPos.at(2).x - _box.at(3)->getPos().x) < 1.6f &&
				abs(_answerPos.at(2).z - _box.at(3)->getPos().z) < 1.6f;

			bool box3Correct =
				abs(_answerPos.at(1).x - _box.at(2)->getPos().x) < 1.6f &&
				abs(_answerPos.at(1).z - _box.at(2)->getPos().z) < 1.6f;

			bool box4Correct =
				abs(_answerPos.at(0).x - _box.at(0)->getPos().x) < 1.6f &&
				abs(_answerPos.at(0).z - _box.at(0)->getPos().z) < 1.6f;

			if (box1Correct && box2Correct && box3Correct && box4Correct) {
				return true;
			}
			return false;
		}

	private:
		IGameEventHandler* _gameEvent;
		std::function<void()> _callback;
		Model* _boxGrid;
		std::array<Wall*, 4> _gridBorder;
		std::array<Block*, BOX_NUM> _box;
		bool _isSolved = false;
		const float _startX = 24.5f;
		const float _startY = 1.0f;
		const float _startZ = 2.0f;
		const std::array<Float3, 4> _answerPos {
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