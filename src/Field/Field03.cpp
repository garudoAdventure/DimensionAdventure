#include "Field.h"
#include "./Object/Door.h"
#include "./Object/PlayerFallPoint.h"
#include "./Object/SavePoint.h"
#include "./Item/Crystal.h"

Field03::Field03(IGameEventHandler* gameEvent) : Field("./assets/map/map3.csv") {
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), 4, gameEvent,
			MathTool::getCoordPos({ 2.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 20.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 20.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 35.0f, 1.5f, 10.0f }), 5, gameEvent,
			MathTool::getCoordPos({ 3.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new PlayerFallPoint(
			MathTool::getCoordPos({ 20.0f, -15.0f, 5.0f }),
			{ 40.0f * 3.0f, 1.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
	}
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 24.0f, 1.5f, 5.0f }),
			{ 1.0f * 3.0f, 5.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 31.0f, 1.5f, 5.0f }),
			{ 1.0f * 3.0f, 5.0f * 3.0f, 10.0f * 3.0f },
			gameEvent
		));
	}

	Field::load();
}