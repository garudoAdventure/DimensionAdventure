#include "Field.h"
#include "MathTool.h"
#include "Texture.h"
#include "Block.h"
#include "FloorBase.h"
#include "CSVParser.h"
#include "Color.h"

Field::Field(const char* file) : filePath(file) {
	_layer[0] = new Layer();
	_layer[1] = new Layer();
	_layer[2] = new Layer();
	_layer[3] = new Layer();
}

void Field::load() {
	CSVParser parser;
	const std::vector<BlockInfo> map = parser.loadCSV(filePath);
	Float4 layerColor[4] = {
		Color::white,
		Color::lightRed,
		Color::lightGreen,
		Color::lightBlue,
	};
	for (BlockInfo block : map) {
		const int layerIdx = block.pos.w;
		const Float3 pos = MathTool::getCoordPos({
			(float)block.pos.x, (float)block.pos.y, (float)block.pos.z
		});
		switch (block.type) {
			// Floor
			case 1:
			case 2:
			case 3:
				_layer[layerIdx]->addGameObj(new Block(pos, block.scale, layerColor[layerIdx]));
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

void Field::collisionCheck(bool is2D) {
	_layer[PLAYER.getCurrentLayer()]->collisionCheck(is2D);
}

Field::~Field() {
	delete _layer[0];
	delete _layer[1];
	delete _layer[2];
}