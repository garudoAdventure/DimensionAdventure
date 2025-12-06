#include "Field.h"
#include "Door.h"
#include "Crystal.h"

Field54::Field54(IGameEventHandler* gameEvent) : Field("./map/map5.csv") {
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 55.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 5.0f, 1.1f, 5.0f })
		));
	}

	_layer[0]->addGameObj(new GreenCrystal(
		MathTool::getCoordPos({ 25.0f, 2.5f, 5.0f }),
		gameEvent
	));

	Field::load();
}