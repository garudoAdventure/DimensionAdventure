#include "Field.h"
#include "MathTool.h"
#include "Texture.h"
#include "Block.h"
#include "CSVParser.h"

Field::Field(const char* file) {
	_layer[0] = new Layer("./assets/bg1.png");
	_layer[1] = new Layer("./assets/bg2.png");
	_layer[2] = new Layer("./assets/bg3.png");
	
	CSVParser parser;
	const std::vector<BlockInfo> map = parser.loadCSV(file);
	for (BlockInfo block : map) {
		const int layerIdx = block.pos.w;
		const Float3 pos = MathTool::getCoordPos({
			block.pos.x + (float)((int)block.size.x % 2 == 0 ? 0.5f : 0),
			block.pos.y + (float)((int)block.size.y % 2 == 0 ? 0.5f : 0),
			block.pos.z - (float)((int)block.size.z % 2 == 0 ? 0.5f : 0)
		});
		const int floorTexID[3] = {
			TEXTURE.loadTexture("./assets/tile1.png"),
			TEXTURE.loadTexture("./assets/tile2.png"),
			TEXTURE.loadTexture("./assets/tile3.png")
		};
		switch (block.type) {
			case 1:
				_layer[layerIdx]->getBlockManager()->add(new Block(pos, block.size, floorTexID[layerIdx]));
				break;
			case 2:
			case 3:
				_layer[layerIdx]->getBlockManager()->add(new Block(pos, block.size));
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