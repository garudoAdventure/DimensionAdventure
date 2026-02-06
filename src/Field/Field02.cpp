#include "Field.h"
#include "./Render/Model.h"
#include "./Object/Door.h"
#include "./Object/Block.h"
#include "./Object/PlayerFallPoint.h"
#include "./Object/SavePoint.h"
#include "./Object/Epigraph.h"
#include "./Item/DimensionRing.h"
#include "./Item/RemoteControl.h"

Field02::Field02(IGameEventHandler* gameEvent) : Field("./assets/map/map2.csv") {
	Model* hintWall[LAYER_NUM] = {
		MODEL.loadModel("./assets/model/hintBlock.fbx"),
		MODEL.loadModel("./assets/model/doorHintBlock.fbx"),
		MODEL.loadModel("./assets/model/boxHintBlock.fbx"),
		MODEL.loadModel("./assets/model/layerHintBlock.fbx")
	};
	
	for (int i = 0; i < LAYER_NUM; i++) {
		_layer[i]->addGameObj(new OpenedDoor(
			MathTool::getCoordPos({ 3.0f, 1.5f, 10.0f }), 1, gameEvent,
			MathTool::getCoordPos({ 35.0f, 1.1f, 5.0f })
		));
		_layer[i]->addGameObj(new HintBlock(
			MathTool::getCoordPos({ 14.0f, 3.0f, 5.0f }),
			{ 1.0f, 1.0f, 1.0f }, Color::layerColor[i],
			hintWall[i]
		));
		_layer[i]->addGameObj(new PlayerFallPoint(
			MathTool::getCoordPos({ 29.0f, -15.0f, 5.0f }),
			Float3{ 58.0f, 1.0f, 10.0f } * 3,
			gameEvent
		));
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 10.0f, 1.5f, 5.0f }),
			Float3{ 1.0f, 15.0f, 10.0f } * 3,
			gameEvent
		));
		_layer[i]->addGameObj(new SavePoint(
			MathTool::getCoordPos({ 34.0f, 1.5f, 5.0f }),
			Float3{ 1.0f, 15.0f, 10.0f } * 3,
			gameEvent
		));
	}

	_layer[LayerType::WHITE]->addGameObj(new DimensionRing(
		MathTool::getCoordPos({ 12.0f, 2.5f, 5.0f }),
		gameEvent
	));
	_layer[LayerType::WHITE]->addGameObj(new RemoteControl(
		MathTool::getCoordPos({ 39.0f, 4.5f, 5.0f }),
		gameEvent
	));
	_layer[LayerType::WHITE]->addGameObj(new Epigraph(gameEvent,
		MathTool::getCoordPos({ 8.0f, 2.0f, 10.0f }),
		Color::white,
		L"「三つの水晶を集めれば、奥への道が開かれる」と記されている。"
	));
	
	Field::load();
}