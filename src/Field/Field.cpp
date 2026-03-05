#include "Field.h"
#include "./Render/ModelManager.h"
#include "./Object/Block.h"
#include "./Object/Wall.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include "./Utils/CSVParser.h"

Field::Field(const char* file) : filePath(file) {
	_layer.at(0) = new Layer();
	_layer.at(1) = new Layer();
	_layer.at(2) = new Layer();
	_layer.at(3) = new Layer();

	// Air wall
	for (int i = 0; i < LAYER_NUM; i++) {
		_layer.at(i)->addGameObj(new Wall(
			MathTool::getCoordPos({ 19.5f, 10.0f, 11.0f }),
			{ 40.0f * 3, 20.0f * 3, 1.0f * 3 }
		));
		_layer.at(i)->addGameObj(new Wall(
			MathTool::getCoordPos({ 19.5f, 10.0f, -1.0f }),
			{ 40.0f * 3, 20.0f * 3, 1.0f * 3 }
		));
		_layer.at(i)->addGameObj(new Wall(
			MathTool::getCoordPos({ -1.0f, 10.0f, 5.0f }),
			{ 1.0f * 3, 20.0f * 3, 11.0f * 3 }
		));
		_layer.at(i)->addGameObj(new Wall(
			MathTool::getCoordPos({ 40.0f, 10.0f, 5.0f }),
			{ 1.0f * 3, 20.0f * 3, 11.0f * 3 }
		));
	}
}

void Field::load() {
	CSVParser parser;
	const std::vector<BlockInfo> map = parser.loadCSV(filePath);
	for (BlockInfo block : map) {
		const int layerIdx = block.pos.w;
		const Float3 pos = MathTool::getCoordPos({
			(float)block.pos.x + ((int)block.scale.x % 2 == 0 ? 0.5f : 0.0f),
			(float)block.pos.y + ((int)block.scale.y % 2 == 0 ? 0.5f : 0.0f),
			(float)block.pos.z + ((int)block.scale.z % 2 == 0 ? 0.5f : 0.0f)
		});
		switch (block.type) {
			// Floor
			case 1:
			case 2:
			case 3:
				_layer.at(layerIdx)->addGameObj(
					new Block(pos, block.scale, Color::layerColor.at(layerIdx), MODEL.loadModel("./assets/model/block.fbx"))
				);
				break;
		}
	}
}

void Field::update(int currentLayer) {
	_layer.at(currentLayer)->update();
}

void Field::draw(int currentLayer) {
	_layer.at(currentLayer)->draw();
}

void Field::drawBillboard(int currentLayer) {
	_layer.at(currentLayer)->drawBillboard();
}

void Field::collisionCheck(bool is2D) {
	_layer.at(PLAYER.getCurrentLayer())->collisionCheck(is2D);
}

Field::~Field() {
	delete _layer.at(0);
	delete _layer.at(1);
	delete _layer.at(2);
	delete _layer.at(3);
}