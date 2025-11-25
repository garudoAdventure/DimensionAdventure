#include "Field.h"
#include "Door.h"
#include "MovingBoxArea.h"

Field05::Field05(IGameEventHandler* gameEvent) : Field("./map/map5.csv") {
	_layer[0]->addStagePuzzle(new MovingBoxArea(gameEvent));

	Door* door = new OpenedDoor(
		MathTool::getCoordPos({ 5.0f, 2.9f, 0.0f }), 6, gameEvent,
		MathTool::getCoordPos({ 5.0f, 2.5f, 0.0f })
	);
	_layer[0]->addGameObj(door);
	
	Field::load();
}