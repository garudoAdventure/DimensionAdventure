#include "Field.h"
#include "Door.h"
#include "Math.h"

Field00::Field00(IGameEventHandler* gameEvent) : Field("./map/map0.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 53.0f, 2.4f, 0.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 5.0f, 2.0f, 4.0f })
		);
		_layer[i]->getDoorManager()->add(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new LockedDoor(
			MathTool::getCoordPos({ 5.0f, 2.4f, 0.0f }), gameEvent
		);
		_layer[i]->getDoorManager()->add(door);
	}
}