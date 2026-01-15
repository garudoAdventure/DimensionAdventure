#include "Field.h"
#include "./Object/Door.h"
#include "./Item/Crystal.h"

Field54::Field54(IGameEventHandler* gameEvent) : Field("./assets/map/map5.csv"), _gameEvent(gameEvent) {
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 35.0f, 1.1f, 5.0f })
		));
	}

	_layer[LayerType::RED]->addGameObj(new GreenCrystal(
		MathTool::getCoordPos({ 39.0f, 4.5f, 5.0f }),
		gameEvent
	));

	Field::load();
}