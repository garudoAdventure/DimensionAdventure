#include "Field.h"
#include "Door.h"
#include "Crystal.h"
#include "ItemBox.h"

Field01::Field01(IGameEventHandler* gameEvent) : Field("./map/map1.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 1.5f, 10.0f }), 0, gameEvent,
			MathTool::getCoordPos({ 53.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 29.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 29.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 53.0f, 1.5f, 10.0f }), 2, gameEvent,
			MathTool::getCoordPos({ 5.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	
	Field::load();
}