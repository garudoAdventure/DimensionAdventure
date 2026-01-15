#include "Field.h"
#include "./Object/Door.h"
#include "./Item/Crystal.h"

Field01::Field01(IGameEventHandler* gameEvent) : Field("./assets/map/map1.csv") {
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 2.0f, 1.5f, 10.0f }), 0, gameEvent,
			MathTool::getCoordPos({ 35.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 20.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 20.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 35.0f, 1.5f, 10.0f }), 2, gameEvent,
			MathTool::getCoordPos({ 3.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 20.0f, 15.5f, 10.0f }), 6, gameEvent,
			MathTool::getCoordPos({ 3.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	
	Field::load();
}