#include "Field.h"
#include "Door.h"
#include "Statue.h"

Field06::Field06(IGameEventHandler* gameEvent) : Field("./map/map5.csv") {
	Door* door = new OpenedDoor(
		MathTool::getCoordPos({ 5.0f, 2.9f, 0.0f }), 5, gameEvent,
		MathTool::getCoordPos({ 5.0f, 2.5f, 0.0f })
	);
	_layer[0]->addGameObj(door);

	_layer[0]->addGameObj(new RedBirdStatue(
		MathTool::getCoordPos({ 10.0f, 2.9f, 0.0f }), gameEvent));
	_layer[0]->addGameObj(new BlueDragonStatue(
		MathTool::getCoordPos({ 20.0f, 2.9f, 0.0f }), gameEvent));
	_layer[0]->addGameObj(new WhiteTigerStatue(
		MathTool::getCoordPos({ 30.0f, 2.9f, 0.0f }), gameEvent));
	_layer[0]->addGameObj(new GreenTurtleStatue(
		MathTool::getCoordPos({ 40.0f, 2.9f, 0.0f }), gameEvent));
	
	Field::load();
}