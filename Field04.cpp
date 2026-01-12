#include "Field.h"
#include "Door.h"
#include "Block.h"
#include "Color.h"
#include "Crystal.h"
#include "Epigraph.h"
#include "PlayerFallPoint.h"
#include "SavePoint.h"

Field04::Field04(IGameEventHandler* gameEvent) : Field("./map/map4.csv") , _gameEvent(gameEvent) {
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 2.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 3.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}

	Model* floorModel = MODEL.loadModel("./assets/model/transposeBox.fbx");
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new MovingFloor(
			MathTool::getCoordPos({ 15.0f, 0.0f, 5.0f }),
			MathTool::getCoordPos({ 10.0f, 0.0f, 5.0f }),
			{ 3.0f, 1.0f, 3.0f },
			_layerColor[i], floorModel
		));
		_layer[i]->addGameObj(new MovingFloor(
			MathTool::getCoordPos({ 15.0f, 0.0f, 8.0f }),
			MathTool::getCoordPos({ 20.0f, 0.0f, 8.0f }),
			{ 3.0f, 1.0f, 3.0f },
			_layerColor[i], floorModel
		));
		_layer[i]->addGameObj(new MovingFloor(
			MathTool::getCoordPos({ 28.0f, 0.0f, 1.0f }),
			MathTool::getCoordPos({ 23.0f, 0.0f, 1.0f }),
			{ 3.0f, 1.0f, 3.0f },
			_layerColor[i], floorModel
		));
		_layer[i]->addGameObj(new MovingFloor(
			MathTool::getCoordPos({ 31.0f, 0.0f, 5.0f }),
			MathTool::getCoordPos({ 31.0f, 5.0f, 5.0f }),
			{ 3.0f, 1.0f, 3.0f },
			_layerColor[i], floorModel
		));
	}

	_layer[LayerType::WHITE]->addGameObj(new RedCrystal(
		MathTool::getCoordPos({ 39.0f, 3.5f, 5.0f }),
		gameEvent
	));

	_layer[LayerType::WHITE]->addGameObj(new Epigraph(gameEvent,
		MathTool::getCoordPos({ 7.0f, 2.0f, 10.0f }),
		Color::lightRed,
		L"「お宝は奥にある」と記されている。"
	));

	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new PlayerFallPoint(
			MathTool::getCoordPos({ 29.0f, -15.0f, 5.0f }),
			{ 58.0f * 3.0f, 1.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
	}
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 7.0f, 1.5f, 5.0f }),
			{ 1.0f * 3.0f, 15.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 34.0f, 1.5f, 5.0f }),
			{ 1.0f * 3.0f, 15.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
	}
	
	Field::load();
}