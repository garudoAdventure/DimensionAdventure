#include "Field.h"
#include "Door.h"
#include "Math.h"
#include "ModelManager.h"

Field00::Field00(IGameEventHandler* gameEvent) : Field("./map/map0.csv") {	
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 53.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 5.0f, 2.5f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new LockedDoor(
			MathTool::getCoordPos({ 1.0f, 1.5f, 10.0f }), gameEvent
		);
		_layer[i]->addGameObj(door);
	}
	
	Field::load();
}