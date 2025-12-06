#include "Field.h"
#include "Door.h"
#include "DimensionBangle.h"

Field02::Field02(IGameEventHandler* gameEvent) : Field("./map/map2.csv") {
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 55.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}
	_layer[3]->addGameObj(new DimensionBangle(
		MathTool::getCoordPos({ 23.0f, 2.5f, 5.0f }),
		gameEvent
	));
	
	Field::load();
}