#include "Field.h"
#include "Door.h"
#include "ClimbableObj.h"

Field03::Field03(IGameEventHandler* gameEvent) : Field("./map/map3.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 55.0f, 14.0f, 0.0f }), 2, gameEvent,
			MathTool::getCoordPos({ 5.0f, 3.5f, 4.0f })
		);
		_layer[i]->getDoorManager()->add(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 55.0f, 4.0f, 0.0f }), 4, gameEvent,
			MathTool::getCoordPos({ 5.0f, 3.5f, 4.0f })
		);
		_layer[i]->getDoorManager()->add(door);
	}
	{
		ClimbableObj* ladder = new ClimbableObj(MathTool::getCoordPos({ 47.0f, 6.5f, 4.0f }), { 1.0f, 12.0f, 1.0f });
		_layer[0]->getClimbableObjManager()->add(ladder);
	}
}