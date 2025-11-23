#pragma once

#include "Block.h"
#include "MathTool.h"
#include "ModelManager.h"
#include "IStagePuzzle.h"
#include "IGameEventHandler.h"
#include "CubeRenderer.h"
#include "ItemBox.h"
#include "Crystal.h"
#include "Dialog.h"

#define BOX_NUM	(4)

class MovingBoxArea : public IStagePuzzle {
	public:
		MovingBoxArea(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			boxGrid = MODEL.loadModel("./assets/model/boxGrid.fbx");
			Model* boxModel[4] = {
				MODEL.loadModel("./assets/model/boxNum4.fbx"),
				MODEL.loadModel("./assets/model/boxNum1.fbx"),
				MODEL.loadModel("./assets/model/boxNum3.fbx"),
				MODEL.loadModel("./assets/model/boxNum2.fbx"),
			};
			const Int2 boxCoord[4] = {
				{ 3, 0 }, { 2, 1 }, { 1, 2 }, { 0, 3 }
			};
			for (int i = 0; i < 4; i++) {
				box[i] = new MovableBox(MathTool::getCoordPos({
					startX - boxCoord[i].x * 2, startY, startZ + boxCoord[i].y * 2
				}), boxModel[i]);
			}
			gridBorder[0] = new Block(
				MathTool::getCoordPos({ startX - 3.0f, startY - 1.0f, startZ - 1.0f }),
				MODEL.loadModel("./assets/model/gridBorderHorizontal.fbx")
			);
			gridBorder[1] = new Block(
				MathTool::getCoordPos({ startX - 3.0f, startY - 1.0f, startZ + 7.0f }),
				MODEL.loadModel("./assets/model/gridBorderHorizontal.fbx")
			);
			gridBorder[2] = new Block(
				MathTool::getCoordPos({ startX + 1.0f, startY - 1.0f, startZ + 3.0f }),
				MODEL.loadModel("./assets/model/gridBorderVerticle.fbx")
			);
			gridBorder[3] = new Block(
				MathTool::getCoordPos({ startX - 7.0f, startY - 1.0f, startZ + 3.0f }),
				MODEL.loadModel("./assets/model/gridBorderVerticle.fbx")
			);

			itemBox = new ItemBox(
				MathTool::getCoordPos({ startX - 20.0f, startY, 5.0f }),
				new Crystal(), _gameEvent
			);
			itemBox->setLocked(true);
		}

		void update() override {			
			for (int i = 0; i < BOX_NUM; i++) {
				box[i]->update();
			}
			itemBox->update();
			if (!_isSolved) {
				if (checkAnswer()) {
					_isSolved = true;
					itemBox->setLocked(false);
					_gameEvent->addEvent(new ShowDialogEvent(
						new NormalDialog({
							"ガチャッ",
							"宝箱のロックが解除されました"
						})
					));
				}
			}
		}

		void draw() override {
			boxGrid->draw(MathTool::getCoordPos({
				30.5f, 1.0f, 5.0f
			}), { 0.0f, 0.0f, 0.0f });
			for (int i = 0; i < BOX_NUM; i++) {
				gridBorder[i]->draw();
				box[i]->draw();
			}
			itemBox->draw();
		}

		void collide(Player* player, bool is2D) override {
			itemBox->trigger(player, is2D);
			for (int i = 0; i < BOX_NUM; i++) {
				if (MathTool::checkCollision(player->getBox(), box[i]->getBox(), is2D)) {
					// Player Hit Block, update block position
					player->hitObj(box[i], _isSolved);

					// Check collision with other box
					for (int j = 0; j < BOX_NUM; j++) {
						if (i == j) continue;
						if (MathTool::checkCollision(box[i]->getBox(), box[j]->getBox(), is2D)) {
							box[i]->hitObj(box[j]);
						}
					}

					// Hit border
					for (int j = 0; j < 4; j++) {
						if (MathTool::checkCollision(box[i]->getBox(), gridBorder[j]->getBox(), is2D)) {
							box[i]->hitObj(gridBorder[j]);
						}
					}

					// Update player position
					player->hitObj(box[i]);
				}
			}
		}

		bool checkAnswer() {
			bool box1Correct =
				abs(answerPos[0].x - box[1]->getPos().x) < 1.2f &&
				abs(answerPos[0].z - box[1]->getPos().z) < 1.2f;

			bool box2Correct =
				abs(answerPos[1].x - box[3]->getPos().x) < 1.2f &&
				abs(answerPos[1].z - box[3]->getPos().z) < 1.2f;

			bool box3Correct =
				abs(answerPos[2].x - box[2]->getPos().x) < 1.2f &&
				abs(answerPos[2].z - box[2]->getPos().z) < 1.2f;

			bool box4Correct =
				abs(answerPos[3].x - box[0]->getPos().x) < 1.2f &&
				abs(answerPos[3].z - box[0]->getPos().z) < 1.2f;

			if (box1Correct && box2Correct && box3Correct && box4Correct) {
				return true;
			}
			return false;
		}

	private:
		const float startX = 33.5f;
		const float startY = 2.0f;
		const float startZ = 2.0f;
		IGameEventHandler* _gameEvent;
		Model* boxGrid;
		Block* gridBorder[4];
		MovableBox* box[BOX_NUM];
		ItemBox* itemBox;
		const Float3 answerPos[4] = {
			MathTool::getCoordPos({ startX - 2 * 2.0f, startY, startZ + 0 * 2.0f }),
			MathTool::getCoordPos({ startX - 0 * 2.0f, startY, startZ + 1 * 2.0f }),
			MathTool::getCoordPos({ startX - 3 * 2.0f, startY, startZ + 2 * 2.0f }),
			MathTool::getCoordPos({ startX - 1 * 2.0f, startY, startZ + 3 * 2.0f }),
		};
		bool _isSolved = false;
		// Answer
		// | { 0,     (2-4), 0,     0     }
		// | { 0,     0,     0,     (4-3) }  
		// | { (1-2), 0,     0,     0     }  ↑
		// | { 0,     0,     (3-1), 0     }  
		//                →
};