#include "Field.h"
#include "Door.h"
#include "Block.h"
#include "ModelManager.h"

Field04::Field04(IGameEventHandler* gameEvent) : Field("./map/map4.csv") {
	for (int i = 0; i < 3; i++) {
		Door* door = new OpenedDoor(
			MathTool::getCoordPos({ 5.0f, 4.0f, 0.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 55.0f, 3.5f, 4.0f })
		);
		_layer[i]->addGameObj(door);
	}
	//for (int i = 0; i < 3; i++) {
	//	Door* door = new OpenedDoor(
	//		MathTool::getCoordPos({ 55.0f, 4.0f, 0.0f }), 1, gameEvent,
	//		{ 36.0f, -5.0f, 0.0f }
	//	);
	//	_layer[i]->getDoorManager()->add(door);
	//}
	_layer[0]->addGameObj(new MovingFloor(MathTool::getCoordPos({ 12.0f, 0.0f, 4.0f }), { 0.1f, 0.0f, 0.0f }, MODEL.loadModel("./assets/model/block.fbx")));
	_layer[0]->addGameObj(new MovingFloor(MathTool::getCoordPos({ 46.0f, 0.0f, 4.0f }), { -0.1f, 0.0f, 0.0f }, MODEL.loadModel("./assets/model/block.fbx")));
	
	Field::load();
}