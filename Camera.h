#pragma once

#include "MathStruct.h"
#include "Player.h"

#define CAMERA_2D_MIN_X	(0.0f)
#define CAMERA_2D_MAX_X	(73.0f)
#define CAMERA_3D_MIN_X	(-42.0f)
#define CAMERA_3D_MAX_X	(52.0f)

class CameraController;

class Camera {
  public:
		Camera();
		~Camera();
		void draw();
		void moveCamera(Player* player);
		void transformDimension(Player* player);
		void set2DPos(Float2 pos);
		Float2 get2DPos();
		bool is2D() {
			return _is2DMode;
		}

  private:
		Float3 _eye = { 0.0f, 0.0f, -30.0f };
		Float3 _newEye = { 0.0f, 0.0f, -30.0f };
		Float3 _focus = { 0.0f, 0.0f, 0.0f };
		Float3 _newFocus = { 0.0f, 0.0f, 0.0f };
		CameraController* cameraController;
		CameraController* cameraController2D;
		CameraController* cameraController3D;
		XMMATRIX projMatrix;
		bool _isTransforming = false;
		bool _is2DMode = true;
		int count = 30;
};

class CameraController {
	public:
		virtual void move(Player* player, Float3 &eye, Float3 &focus) = 0;
		virtual void rotate(Player* player, Float3& eye, Float3& focus, XMMATRIX& projMat, float step) = 0;
};

class CameraController2D : public CameraController {
	public:
		CameraController2D() = default;
		void move(Player* player, Float3& eye, Float3& focus) override;
		void rotate(Player* player, Float3& eye, Float3& focus, XMMATRIX& projMat, float step) override;
};

class CameraController3D : public CameraController {
	public:
		CameraController3D() = default;
		void move(Player* player, Float3& eye, Float3& focus) override;
		void rotate(Player* player, Float3& eye, Float3& focus, XMMATRIX& projMat, float step) override;
};