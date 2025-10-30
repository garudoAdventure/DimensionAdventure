#pragma once

#include "MathStruct.h"
#include <vector>
#include <list>

template<class T>
class Quadtree {
  public:
	void regist(Float2 min, Float2 max, TreeObject<T>* obj) {
	  DWORD elem = getMortonNumber(min, max);
	  if (elem < _cellNum) {
		if (!cellArray[elem]) {
		  createCell(elem);
		}
		cellArray[elem]->push(obj);
	  }
	}
	DWORD getMortonNumber(Float2 min, Float2 max) {
	  DWORD LT = getPointElement(min);
	  DWORD RB = getPointElement(max);
	  DWORD Def = RB ^ LT;
	  unsigned int hiLevel = 1;
	  for (int i = 0; i < _level; i++) {
		DWORD check = (Def >> (i * 2)) & 0x3;
		if (check != 0) {
		  hiLevel = i + 1;
		}
	  }
	  DWORD spaceNum = RB >> (hiLevel * 2);
	  DWORD addNum = (pow(4, _level - hiLevel) - 1) / 3;
	  spaceNum += addNum;
	  if (spaceNum > _cellNum) {
		return 0xffffffff;
	  }
	  return spaceNum;
	}
	void getAllSpaceObject(DWORD elem, std::vector<T*>& colVec) {
	  colVec.clear();
	  if (cellArray[0] == nullptr) {
		return;
	  }
	  std::list<T*> colList;
	  getParentCollisionList(elem, colVec, colList);
	  getChildCollisionList(elem, colVec, colList);
	}
	void draw() {
	  int pow = 1 << _level;
	  int number;
	  for (int y = 0; y < pow; y++) {
		for (int x = 0; x < pow; x++) {
		  number = get2DMortonNumber(x, y);
		  SPRITE.draw(
			{
			  minRange.x + unit.x * 0.5f + unit.x * x,
			  minRange.y + unit.y * 0.5f + unit.y * y
			},
			unit, { 1.0f, 1.0f, 1.0f, 0.2f }
		  );
		}
	  }
	}

  private:
	unsigned int _level = 0;
	unsigned int _cellNum;
	Cell<T>** cellArray;
	Float2 range;
	Float2 maxRange;
	Float2 minRange;
	Float2 unit;

	void getChildCollisionList(DWORD elem, std::vector<T*>& colVec, std::list<T*>& colList) {
	  if (cellArray[elem]) {
		TreeObject<T>* treeObj = cellArray[elem]->getFirstNode();
		if (treeObj) {
		  colVec.emplace_back(treeObj->obj);
		  TreeObject<T>* nextObj = treeObj->nextNode;
		  while (nextObj != nullptr) {
			colVec.emplace_back(nextObj->obj);
			nextObj = nextObj->nextNode;
		  }
		}
	  }

	  DWORD childElem;
	  for (int i = 0; i < 4; i++) {
		childElem = elem * 4 + 1 + i;
		if (childElem < _cellNum && cellArray[childElem]) {
		  getChildCollisionList(childElem, colVec, colList);
		}
	  }
	}

	void getParentCollisionList(DWORD elem, std::vector<T*>& colVec, std::list<T*>& colList) {
	  if (elem != 0) {
		DWORD parentElem = (elem - 1) >> 2;
		if (cellArray[parentElem]) {
		  getParentCollisionList(parentElem, colVec, colList);

		  TreeObject<T>* treeObj = cellArray[parentElem]->getFirstNode();
		  if (treeObj) {
			colVec.emplace_back(treeObj->obj);
			TreeObject<T>* nextObj = treeObj->nextNode;
			while (nextObj != nullptr) {
			  colVec.emplace_back(nextObj->obj);
			  nextObj = nextObj->nextNode;
			}
		  }
		}
	  }
	}

	void createCell(DWORD elem) {
	  while (!cellArray[elem]) {
		cellArray[elem] = new Cell<T>;
		elem = (elem - 1) >> 2;
		if (elem >= _cellNum) {
		  break;
		}
	  }
	}
	DWORD getPointElement(Float2 p) {
	  return get2DMortonNumber(
		(BYTE)((p.x - minRange.x) / unit.x),
		(BYTE)((p.y - minRange.y) / unit.y)
	  );
	}
	DWORD get2DMortonNumber(BYTE x, BYTE y) {
	  DWORD bitX = bitSeperateFor2D(x);
	  DWORD bitY = bitSeperateFor2D(y);
	  return bitX | bitY << 1 ;
	}
	DWORD bitSeperateFor2D(BYTE n) {
	  DWORD s = n;
	  s = (s | s << 8) & 0x00ff00ff;
	  s = (s | s << 4) & 0x0f0f0f0f;
	  s = (s | s << 2) & 0x33333333;
	  s = (s | s << 1) & 0x55555555;
	  return s;
	}

  public:
	static void CreateInstance() {
	  DeleteInstance();
	  s_instance = new Quadtree();
	}
	static void DeleteInstance() {
	  delete s_instance;
	  s_instance = nullptr;
	}
	static Quadtree& GetInstance() {
	  return *s_instance;
	}
  private:
	Quadtree() {
	  _level = 3;
	  _cellNum = (pow(4, _level) - 1) / 3;
	  cellArray = new Cell<T>*[_cellNum];
	  ZeroMemory(cellArray, sizeof(Cell<T>*) * _cellNum);

	  minRange = { -0.2f, -1.0f };
	  maxRange = { 4.0f, 1.0f };
	  range = maxRange - minRange;
	  unit = range / (float)(1 << _level);
	}
	~Quadtree() {
	  for (int i = 0; i < _cellNum; i++)
		if (cellArray[i] != NULL) {
		  delete cellArray[i];
		}
	  delete[] cellArray;
	}
	static inline Quadtree* s_instance;
};

#define QUADTREE Quadtree<Cube>::GetInstance()