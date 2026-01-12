#pragma once

#include "DirectX.h"

#define PI	3.1415926

struct Float2 {
  float x;
  float y;

  Float2 operator+(const Float2& r) {
	  return { x + r.x, y + r.y };
  }
  Float2& operator+=(const Float2& r) {
    x += r.x;
    y += r.y;
    return *this;
  }
  Float2 operator-(const Float2& r) {
	  return { x - r.x, y - r.y };
  }
  Float2& operator-=(const Float2& r) {
    x -= r.x;
    y -= r.y;
    return *this;
  }
  Float2 operator*(const float r) {
    return { x * r, y * r };
  }
  Float2 operator/(const float r) {
	  return { x / r, y / r };
  }
};

struct Int2 {
  int x;
  int y;

  Int2 operator+(const Int2& r) {
    return { x + r.x, y + r.y };
  }
  Int2& operator+=(const Int2& r) {
    x += r.x;
    y += r.y;
    return *this;
  }
  Int2 operator-(const Int2& r) {
    return { x - r.x, y - r.y };
  }
  Int2& operator-=(const Int2& r) {
    x -= r.x;
    y -= r.y;
    return *this;
  }
  Int2 operator*(const int r) {
    return { x * r, y * r };
  }
  Int2 operator/(const int r) {
    return { x / r, y / r };
  }
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

  Int3 operator+(const Int3& r) {
    return { x + r.x, y + r.y, z + r.z };
  }
  Int3& operator+=(const Int3& r) {
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
  }
  Int3 operator-(const Int3& r) {
    return { x - r.x, y - r.y, z - r.z };
  }
  Int3& operator-=(const Int3& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
  }
  Int3 operator*(const int r) {
    return { x * r, y * r, z * r };
  }
  Int3 operator/(const int r) {
    return { x / r, y / r, z / r };
  }
};

struct Float4 {
  float r;
  float g;
  float b;
  float a;

  Float4 operator+(const Float4& s) {
    return { r + s.r, g + s.g, b + s.b, a + s.a };
  }
  Float4& operator+=(const Float4& s) {
    r += s.r;
    g += s.g;
    b += s.b;
    a += s.a;
    return *this;
  }
  Float4 operator-(const Float4& s) {
    return { r - s.r, g - s.g, b - s.b, a - s.a };
  }
  Float4& operator-=(const Float4& s) {
    r -= s.r;
    g -= s.g;
    b -= s.b;
    a -= s.a;
    return *this;
  }
  Float4 operator*(const float s) {
    return { r * s, g * s, b * s, a * s };
  }
  Float4 operator/(const float s) {
    return { r / s, g / s, b / s, a / s };
  }
};

struct Int4 {
  int x;
  int y;
  int z;
  int w;

  Int4 operator+(const Int4& r) {
    return { x + r.x, y + r.y, z + r.z, w + r.w };
  }
  Int4& operator+=(const Int4& r) {
    x += r.x;
    y += r.y;
    z += r.z;
    w += r.w;
    return *this;
  }
  Int4 operator-(const Int4& r) {
    return { x - r.x, y - r.y, z - r.z, w - r.w };
  }
  Int4& operator-=(const Int4& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    w -= r.w;
    return *this;
  }
  Int4 operator*(const int r) {
    return { x * r, y * r, z * r, w * r };
  }
  Int4 operator/(const int r) {
    return { x / r, y / r, z / r, w / r };
  }
};

struct Cube {
  Float3 pos;
  Float3 size;
};

struct BlockInfo {
  Int4 pos;
  Float3 scale;
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
  INT boneIdx[4] = { 0, 0, 0, 0 };
  FLOAT boneWeight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct Transpose {
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX projection;
};

struct Light {
  BOOL enable;
  XMFLOAT3 direction;
  XMFLOAT4 ambient = { 0.4f, 0.4f, 0.4f ,1.0f };
};

struct PixelTime {
  float time;
  Float3 dummy;
};