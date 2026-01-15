#pragma once

#include "./Common/MathStruct.h"

class CameraController;

class Camera {
  public:
		static constexpr float _2D_MIN_X = 0.0f;
		static constexpr float _2D_MAX_X = 72.0f;
		static constexpr float _3D_MIN_X = -52.0f;
		static constexpr float _3D_MAX_X = 42.0f;
		static constexpr float _MAX_Y = 50.0f;
		static constexpr float _Z_DISTANCE = 40.0f;

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
		Float3 _eye = { 0.0f, 0.0f, -_Z_DISTANCE };
		Float3 _newEye = _eye;
		Float3 _focus = { 0.0f, 0.0f, 0.0f };
		Float3 _newFocus = _focus;
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