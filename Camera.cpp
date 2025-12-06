#define NOMINMAX
#include "Camera.h"
#include "Shader.h"
#include "MathTool.h"

Camera::Camera() {
	projMatrix = SHADER.getOrthoMatrix();
	cameraController2D = new CameraController2D();
	cameraController3D = new CameraController3D();
	cameraController = cameraController2D;
}

Camera::~Camera() {
	delete cameraController2D;
	delete cameraController3D;
}

void Camera::draw() {
	SHADER.setView(_eye, _focus);
	SHADER.setProjection(projMatrix);
}

void Camera::transformDimension() {
	if (!_isTransforming) {
		count = 1;
		_isTransforming = true;
		return;
	}
	const int maxCount = 30;
	float step = (float)count / maxCount;

	cameraController->rotate(_eye, _focus, projMatrix, step);

	if (count < maxCount) {
		count++;
	}
	else {
		_is2DMode = !_is2DMode;
		_isTransforming = false;
		cameraController = _is2DMode ? cameraController2D : cameraController3D;
	}
}

void Camera::moveCamera() {
	cameraController->move(_newEye, _newFocus);
	_eye = MathTool::lerp<Float3>(_eye, _newEye, 0.1f);
	_focus = MathTool::lerp<Float3>(_focus, _newFocus, 0.1f);
}

void Camera::setVibration(bool isSet) {
	static int time = 0;
	if (!isSet) {
		_eye.x = _focus.x;
	}
	else {
		_eye.x += sinf(time) * 2;
		time++;
	}
}

void Camera::set2DPos(Float2 pos) {
	_eye.x = pos.x;
	_eye.x = MathTool::clamp(_eye.x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	_focus.x = _eye.x;
	_newEye.x = _eye.x;

	_eye.y = pos.y;
	_eye.y = MathTool::clamp(_eye.y, 0.0f, 20.0f);
	_focus.y = _eye.y;
	_newEye.y = _eye.y;
}

Float2 Camera::get2DPos() {
	if (_is2DMode) {
		return { _eye.x, _eye.y };
	}
	else {
		float eyeX = MathTool::clamp(_eye.x + CAMERA_Z_DISTANCE, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
		return { eyeX, _eye.y - 5.0f };
	}
}

Float3& Camera::getPos() {
	return _eye;
}

void CameraController2D::move(Float3& eye, Float3& focus) {
	eye.x = PLAYER.getPos().x;
	eye.x = MathTool::clamp(eye.x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	focus.x = eye.x;

	eye.y = PLAYER.getPos().y;
	eye.y = MathTool::clamp(eye.y, 0.0f, 20.0f);
	focus.y = eye.y;

	eye.z = -CAMERA_Z_DISTANCE;
}

void CameraController3D::move(Float3& eye, Float3& focus) {
	focus.x = PLAYER.getPos().x;
	focus.x = MathTool::clamp(focus.x, CAMERA_3D_MIN_X + CAMERA_Z_DISTANCE, CAMERA_3D_MAX_X + CAMERA_Z_DISTANCE);
	eye.x = focus.x - CAMERA_Z_DISTANCE;

	focus.y = PLAYER.getPos().y;
	focus.y = MathTool::clamp(focus.y, 0.0f, 20.0f);
	eye.y = focus.y + 5.0f;

	eye.z = 0.0f;
}

void CameraController2D::rotate(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	const float destEyePosX = MathTool::clamp(PLAYER.getPos().x - CAMERA_Z_DISTANCE, CAMERA_3D_MIN_X, CAMERA_3D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.y += 5.0f / 30.0f;
	eye.z += CAMERA_Z_DISTANCE / 30.0f;

	const float srcFocusPosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	const float destFocusPosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_3D_MIN_X + CAMERA_Z_DISTANCE, CAMERA_3D_MAX_X + CAMERA_Z_DISTANCE);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeInQuad<XMMATRIX>(ortho, perspective, step);
}

void CameraController3D::rotate(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::clamp(PLAYER.getPos().x - CAMERA_Z_DISTANCE, CAMERA_3D_MIN_X, CAMERA_3D_MAX_X);
	const float destEyePosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.y -= 5.0f / 30.0f;
	eye.z -= CAMERA_Z_DISTANCE / 30.0f;

	const float srcFocusPosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_3D_MIN_X + CAMERA_Z_DISTANCE, CAMERA_3D_MAX_X + CAMERA_Z_DISTANCE);
	const float destFocusPosX = MathTool::clamp(PLAYER.getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeOutQuad<XMMATRIX>(perspective, ortho, step);
}