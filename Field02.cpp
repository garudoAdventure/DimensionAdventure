#include "Field.h"
#include "Door.h"
#include "Enemy.h"

Field02::Field02(IGameEventHandler* gameEvent) : Field("./map/map2.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 4.0f, 0.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 55.0f, 13.5f, 4.0f })
		);
		_layer[i]->getDoorManager()->add(door);
	}
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 29.0f, 4.0f, 0.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 29.0f, 3.5f, 4.0f })
		);
		_layer[i]->getDoorManager()->add(door);
	}
	{
		Enemy* enemy = new Enemy(MathTool::getCoordPos({ 20.0f, 3.0f, 4.0f }));
		_layer[0]->getEnemyManager()->add(enemy);
	}
}