#include "Field.h"
#include "./Render/ModelManager.h"
#include "./Object/Door.h"

Field00::Field00(IGameEventHandler* gameEvent) : Field("./assets/map/map0.csv") {	
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 35.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 2.0f, 1.1f, 5.0f })
		));
	}
	_layer[0]->addGameObj(new LockedDoor(
		MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), gameEvent
	));
	
	Field::load();
}