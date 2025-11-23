#include "Field.h"
#include "Door.h"
#include "MovingBoxArea.h"

Field05::Field05(IGameEventHandler* gameEvent) : Field("./map/map5.csv") {
	_layer[0]->addStagePuzzle(new MovingBoxArea(gameEvent));
}