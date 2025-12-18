#pragma once

#include "MathStruct.h"
#include "Player.h"

#define CAMERA_2D_MIN_X	(0.0f)
#define CAMERA_2D_MAX_X	(72.0f)
#define CAMERA_3D_MIN_X	(-52.0f)
#define CAMERA_3D_MAX_X	(42.0f)
#define CAMERA_MAX_Y		(50.0f)
#define CAMERA_Z_DISTANCE (40.0f)

class CameraController;

class Camera {
  public:
		Camera();
		~Camera();
		void draw();
		void moveCamera(Float3 targetPos);
		void transformDimension();
		void vibration(bool isSet);
		void set2DPos(Float2 pos);
		Float2 get2DPos();
		Float3& getPos();
		bool is2D() {
			return _is2DMode;
		}

  private:
		Float3 _eye = { 0.0f, 0.0f, -CAMERA_Z_DISTANCE };
		Float3 _newEye = { 0.0f, 0.0f, -CAMERA_Z_DISTANCE };
		Float3 _focus = { 0.0f, 0.0f, 0.0f };
		Float3 _newFocus = { 0.0f, 0.0f, 0.0f };
		CameraController* _cameraController;
		CameraController* _cameraController2D;
		CameraController* _cameraController3D;
		XMMATRIX _projMatrix;
		bool _isTransforming = false;
		bool _is2DMode = true;
		int _count = 30;
};

class CameraController {
	public:
		virtual void move(Float3 targetPos, Float3 &eye, Float3 &focus) = 0;
		virtual void transform(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) = 0;
};

class CameraController2D : public CameraController {
	public:
		CameraController2D() = default;
		void move(Float3 targetPos, Float3& eye, Float3& focus) override;
		void transform(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) override;
};

class CameraController3D : public CameraController {
	public:
		CameraController3D() = default;
		void move(Float3 targetPos, Float3& eye, Float3& focus) override;
		void transform(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) override;
};