#pragma once

#include "MathStruct.h"
#include <vector>
#include <list>

template<class T>
class Octree {
  public:
	void regist(Float3 min, Float3 max, TreeObject<T>* obj) {
	  DWORD elem = getMortonNumber(min, max);
	  if (elem < _cellNum) {
		if (!cellArray[elem]) {
		  createCell(elem);
		}
		cellArray[elem]->push(obj);
	  }
	}
	DWORD getMortonNumber(Float3 min, Float3 max) {
	  DWORD LT = getPointElement(min);
	  DWORD RB = getPointElement(max);
	  DWORD Def = RB ^ LT;
	  unsigned int hiLevel = 1;
	  for (int i = 0; i < _level; i++) {
		DWORD check = (Def >> (i * 3)) & 0x7;
		if (check != 0) {
		  hiLevel = i + 1;
		}
	  }
	  DWORD spaceNum = RB >> (hiLevel * 3);
	  DWORD addNum = (pow(8, _level - hiLevel) - 1) / 7;
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
	  for (int z = 0; z < pow; z++) {
		for (int y = 0; y < pow; y++) {
		  for (int x = 0; x < pow; x++) {
			number = get3DMortonNumber(x, y, z);
			SPRITE.draw(
			  {
				minRange.x + unit.x * 0.5f + unit.x * x,
				minRange.y + unit.y * 0.5f + unit.y * y, 
				minRange.z + unit.z * 0.5f + unit.z * z 
			  },
			  unit, { 1.0f, 1.0f, 1.0f, 0.2f }
			);
		  }
		}
	  }
	}

  private:
	unsigned int _level = 0;
	unsigned int _cellNum;
	Cell<T>** cellArray;
	Float3 range;
	Float3 maxRange;
	Float3 minRange;
	Float3 unit;

	void getChildCollisionList(DWORD elem, std::vector<T*>& colVec, std::list<T*>& colList){
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
	  for (int i = 0; i < 8; i++) {
		childElem = elem * 8 + 1 + i;
		if (childElem < _cellNum && cellArray[childElem]) {
		  getChildCollisionList(childElem, colVec, colList);
		}
	  }
	}

	void getParentCollisionList(DWORD elem, std::vector<T*>& colVec, std::list<T*>& colList) {
	  if (elem != 0) {
		DWORD parentElem = (elem - 1) >> 3;
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
		elem = (elem - 1) >> 3;
		if (elem >= _cellNum) {
		  break;
		}
	  }
	}
	DWORD getPointElement(Float3 p) {
	  return get3DMortonNumber(
		(BYTE)((p.x - minRange.x) / unit.x),
		(BYTE)((p.y - minRange.y) / unit.y),
		(BYTE)((p.z - minRange.z) / unit.z)
	  );
	}
	DWORD get3DMortonNumber(BYTE x, BYTE y, BYTE z) {
	  DWORD bitX = bitSeperateFor3D(x);
	  DWORD bitY = bitSeperateFor3D(y);
	  DWORD bitZ = bitSeperateFor3D(z);
	  return bitX | bitY << 1 | bitZ << 2;
	}
	DWORD bitSeperateFor3D(BYTE n) {
	  DWORD s = n;
	  s = (s | s << 8) & 0x0000f00f;
	  s = (s | s << 4) & 0x000c30c3;
	  s = (s | s << 2) & 0x00249249;
	  return s;
	}

  public:
	static void CreateInstance() {
	  DeleteInstance();
	  s_instance = new Octree();
	}
	static void DeleteInstance() {
	  delete s_instance;
	  s_instance = nullptr;
	}
	static Octree& GetInstance() {
	  return *s_instance;
	}
  private:
	Octree() {
	  _level = 3;
	  _cellNum = (pow(8, _level) - 1) / 7;
	  cellArray = new Cell<T>*[_cellNum];
	  ZeroMemory(cellArray, sizeof(Cell<T>*) * _cellNum);

	  minRange = { -0.2f, -1.0f, -1.0f };
	  maxRange = { 4.0f, 1.0f, 1.0f };
	  range = maxRange - minRange;
	  unit = range / (float)(1 << _level);
	}
	~Octree() {
	  for (int i = 0; i < _cellNum; i++)
		if (cellArray[i] != NULL) {
		  delete cellArray[i];
	  }
	  delete[] cellArray;
	}
	static inline Octree* s_instance;
};

#define OCTREE Octree<Cube>::GetInstance()