#include "Field.h"
#include "Door.h"
#include "Crystal.h"

Field03::Field03(IGameEventHandler* gameEvent) : Field("./map/map3.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 1.5f, 10.0f }), 5, gameEvent,
			MathTool::getCoordPos({ 55.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 29.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 29.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 55.0f, 1.5f, 10.0f }), 4, gameEvent,
			MathTool::getCoordPos({ 5.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}

	Field::load();
}