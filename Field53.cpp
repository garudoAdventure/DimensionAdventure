#include "Field.h"
#include "Door.h"

Field53::Field53(IGameEventHandler* gameEvent) : Field("./map/map5.csv") {
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 55.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 5.0f, 1.1f, 5.0f })
		));
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			_layer[0]->addGameObj(new OpenedDoor(
				MathTool::getCoordPos({ 29.0f + 3 * i, 1.5f, 10.0f - 3 * j }),
				(i == 2 && j == 0 ? 54 : 5), gameEvent,
				MathTool::getCoordPos({ 29.0f + 3 * i, 1.1f, 10.0f - 3 * j - 1.0f })
			));
		}
	}

	Field::load();
}