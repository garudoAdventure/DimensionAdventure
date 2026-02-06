#include "Field.h"
#include "./Object/Door.h"
#include "./Object/Epigraph.h"
#include "./Item/Crystal.h"
#include "./StagePuzzle/MovingBoxArea.h"
#include "./GameEvent/CameraFocusEvent.h"

Field06::Field06(IGameEventHandler* gameEvent) : Field("./assets/map/map5.csv") {
	for (int i = 0; i < LAYER_NUM; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 20.0f, 15.1f, 5.0f })
		));
	}

	_layer[LayerType::GREEN]->addStagePuzzle(
		new MovingBoxArea(gameEvent, [=]() {
			_layer[LayerType::GREEN]->addGameObj(new BlueCrystal(
				MathTool::getCoordPos({ 39.0f, 4.5f, 5.0f }), gameEvent
			));
			gameEvent->addEvent(new CameraFocusEvent(gameEvent,
				MathTool::getCoordPos({ 39.0f, 4.5f, 5.0f })
			));
		})
	);

	_layer[LayerType::GREEN]->addGameObj(new Epigraph(gameEvent,
		MathTool::getCoordPos({ 16.0f, 2.0f, 10.0f }),
		Color::lightBlue,
		L"「四つの箱を正しいマスに置けば、お宝は現れる」と記されている。"
	));

	Field::load();
}