#include "Field.h"
#include "MathTool.h"
#include "Texture.h"
#include "Block.h"
#include "FloorBase.h"
#include "CSVParser.h"
#include "ModelManager.h"

Field::Field(const char* file) : filePath(file) {
	_layer[0] = new Layer();
	_layer[1] = new Layer();
	_layer[2] = new Layer();

	_layer[0]->setBg("./assets/bg1.png");
	_layer[1]->setBg("./assets/bg2.png");
	_layer[2]->setBg("./assets/bg3.png");

	_floor[0] = MODEL.loadModel("./assets/model/floor.fbx");
	_floor[1] = MODEL.loadModel("./assets/model/floor.fbx");
	_floor[2] = MODEL.loadModel("./assets/model/floor.fbx");
}

void Field::load() {
	CSVParser parser;
	const std::vector<BlockInfo> map = parser.loadCSV(filePath);
	for (BlockInfo block : map) {
		const int layerIdx = block.pos.w;
		const Float3 pos = MathTool::getCoordPos({
			block.pos.x + (float)((int)block.size.x % 2 == 0 ? 0.5f : 0),
			block.pos.y + (float)((int)block.size.y % 2 == 0 ? 0.5f : 0),
			block.pos.z - (float)((int)block.size.z % 2 == 0 ? 0.5f : 0)
		});
		switch (block.type) {
			// Floor
			case 1:
				_layer[layerIdx]->getBlockManager()->add(new Block(pos, _floor[layerIdx]));
				break;
			
			// Wall
			case 2:
				_layer[layerIdx]->getBlockManager()->add(new Block(pos, MODEL.loadModel("./assets/model/stoneBlock.fbx")));
				break;

			// Block
			case 3:
				_layer[layerIdx]->getBlockManager()->add(new Block(pos, MODEL.loadModel("./assets/model/box.fbx")));
				break;

			// FloorBase
			case 5:
				_layer[layerIdx]->getFloorBaseManager()->add(new FloorBase(pos, block.size, MODEL.loadModel("./assets/model/floorBase.fbx")));
				break;
		}
	}
}

void Field::update(int currentLayer) {
	_layer[currentLayer]->update();
}

void Field::draw(int currentLayer) {
	_layer[currentLayer]->draw();
}

void Field::collide(Player* player, bool is2D) {
	_layer[player->getCurrentLayer()]->collide(player, is2D);
}

Field::~Field() {
	delete _layer[0];
	delete _layer[1];
	delete _layer[2];
}