#pragma once

#include "DirectX.h"

#define PI	3.1415926

struct Float2 {
  float x;
  float y;

  Float2 operator+(const Float2& r) {
	  return { x + r.x, y + r.y };
  }
  Float2 operator-(const Float2& r) {
	  return { x - r.x, y - r.y };
  }
  Float2 operator/(const float r) {
	  return { x / r, y / r };
  }
};

struct Int2 {
  int x;
  int y;
};

struct Float3 {
  float x;
  float y;
  float z;

  Float3 operator+(const Float3& r) {
	  return { x + r.x, y + r.y, z + r.z };
  }
	Float3& operator+=(const Float3& r) {
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}
  Float3 operator-(const Float3& r) {
	  return { x - r.x, y - r.y, z - r.z };
  }
	Float3& operator-=(const Float3& r) {
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	}
  Float3 operator*(const float r) {
	  return { x * r, y * r, z * r };
  }
  Float3 operator/(const float r) {
	  return { x / r, y / r, z / r };
  }
};

struct Int3 {
  int x;
  int y;
  int z;
};

struct Int4 {
  int x;
  int y;
  int z;
  int w;
};

struct Float4 {
  float r;
  float g;
  float b;
  float a;
};

struct Cube {
  Float3 pos;
  Float3 size;
};

struct BlockInfo {
  Int4 pos;
  Float3 size;
  int type;
};

struct Vertex2D {
  XMFLOAT2 postion;
  XMFLOAT4 color;
  XMFLOAT2 normal;
  XMFLOAT2 texCoord;
};

struct Vertex {
  XMFLOAT3 postion;
  XMFLOAT4 color;
  XMFLOAT3 normal;
  XMFLOAT2 texCoord;
};

struct Transpose {
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX projection;
};

template<class T>
class Cell;

template <class T>
struct TreeObject {
  TreeObject(T* o) : obj(o) {}
  Cell<T>* pCell = nullptr;
  T* obj = nullptr;
  TreeObject<T>* preNode = nullptr;
  TreeObject<T>* nextNode = nullptr;
};

template<class T>
class Cell {
  public:
    Cell() = default;
    bool push(TreeObject<T>* treeObj) {
	    if (treeObj == nullptr) return false;
	    if (treeObj->pCell == this) return false;
	    if (firstNode == nullptr) {
	      firstNode = treeObj;
	    }
	    else {
	      treeObj->nextNode = firstNode;
	      firstNode->preNode = treeObj;
	      firstNode = treeObj;
	    }
	    treeObj->pCell = this;
    }
    TreeObject<T>* getFirstNode() {
	    return firstNode;
    }

  private:
    TreeObject<T>* firstNode = nullptr;
};