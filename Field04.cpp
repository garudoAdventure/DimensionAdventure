#include "Field.h"
#include "Door.h"
#include "Block.h"
#include "Color.h"
#include "Crystal.h"

Field04::Field04(IGameEventHandler* gameEvent) : Field("./map/map4.csv") {
	for (int i = 0; i < 4; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 55.0f, 1.1f, 5.0f })
		);
		_layer[i]->addGameObj(door);
	}

	_layer[3]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 12.0f, 0.0f, 5.0f }),
		MathTool::getCoordPos({ 18.0f, 0.0f, 5.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::white
	));
	_layer[3]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 28.0f, 0.0f, 8.0f }),
		MathTool::getCoordPos({ 18.0f, 0.0f, 8.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::white
	));
	_layer[3]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 39.0f, 0.0f, 1.0f }),
		MathTool::getCoordPos({ 31.0f, 0.0f, 1.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::white
	));
	_layer[3]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 43.0f, 0.0f, 5.0f }),
		MathTool::getCoordPos({ 43.0f, 5.0f, 5.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::white
	));

	_layer[0]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 12.0f, 0.0f, 5.0f }),
		MathTool::getCoordPos({ 18.0f, 0.0f, 5.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::lightRed
	));
	_layer[0]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 28.0f, 0.0f, 8.0f }),
		MathTool::getCoordPos({ 18.0f, 0.0f, 8.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::lightRed
	));
	_layer[0]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 39.0f, 0.0f, 1.0f }),
		MathTool::getCoordPos({ 31.0f, 0.0f, 1.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::lightRed
	));
	_layer[0]->addGameObj(new MovingFloor(
		MathTool::getCoordPos({ 43.0f, 0.0f, 5.0f }),
		MathTool::getCoordPos({ 43.0f, 5.0f, 5.0f }),
		{ 3.0f, 1.0f, 3.0f },
		Color::lightRed
	));

	_layer[3]->addGameObj(new RedCrystal(
		MathTool::getCoordPos({ 55.0f, 2.5f, 5.0f }),
		gameEvent
	));
	
	Field::load();
}