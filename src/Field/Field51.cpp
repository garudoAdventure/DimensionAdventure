#include "Field.h"
#include "./Object/Door.h"
#include "./Object/Epigraph.h"

Field51::Field51(IGameEventHandler* gameEvent) : Field("./assets/map/map5.csv") {
	for (int i = 0; i < 4; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), 3, gameEvent,
			MathTool::getCoordPos({ 35.0f, 1.1f, 5.0f })
		));
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			_layer[LayerType::RED]->addGameObj(new MazeDoor(
				MathTool::getCoordPos({ 20.0f + 3 * i, 1.5f, 10.0f - 3 * j }),
				(i == 1 && j == 2 ? 52 : 5), gameEvent,
				MathTool::getCoordPos({ 20.0f + 3 * i, 1.1f, 10.0f - 3 * j - 1.0f })
			));
		}
	}

	_layer[LayerType::RED]->addGameObj(new Epigraph(gameEvent,
		MathTool::getCoordPos({ 16.0f, 2.0f, 10.0f }),
		Color::lightGreen,
		L"「正しい順番で正しいドアに入れば、道は開かれる」と記されている。"
	));

	Field::load();
}