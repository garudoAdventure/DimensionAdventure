#include "Camera.h"
#include "./Player/Player.h"
#include "./Render/Shader.h"
#include "./Utils/MathTool.h"

Camera::Camera() {
	_projMatrix = SHADER.getOrthoMatrix();
	_cameraController2D = new CameraController2D();
	_cameraController3D = new CameraController3D();
	_cameraController = _cameraController2D;
}

Camera::~Camera() {
	delete _cameraController2D;
	delete _cameraController3D;
}

void Camera::draw() {
	SHADER.setView(_eye, _focus);
	SHADER.setProjection(_projMatrix);
}

void Camera::transformDimension() {
	if (!_isTransforming) {
		_count = 1;
		_isTransforming = true;
		return;
	}
	const int maxCount = 30;
	float step = (float)_count / maxCount;

	_cameraController->transform(_eye, _focus, _projMatrix, step);

	if (_count < maxCount) {
		_count++;
	}
	else {
		_is2DMode = !_is2DMode;
		_isTransforming = false;
		_cameraController = _is2DMode ? _cameraController2D : _cameraController3D;
	}
}

void Camera::moveCamera(Float3 targetPos) {
	_cameraController->move(targetPos, _newEye, _newFocus);
	_eye = MathTool::lerp<Float3>(_eye, _newEye, 0.1f);
	_focus = MathTool::lerp<Float3>(_focus, _newFocus, 0.1f);
}

void Camera::vibration(bool isSet) {
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
	_eye.x = MathTool::clamp(_eye.x, _2D_MIN_X, _2D_MAX_X);
	_focus.x = _eye.x;
	_newEye.x = _eye.x;

	_eye.y = pos.y;
	_eye.y = MathTool::clamp(_eye.y, 0.0f, _MAX_Y);
	_focus.y = _eye.y;
	_newEye.y = _eye.y;
}

Float2 Camera::get2DPos() {
	if (_is2DMode) {
		return { _eye.x, _eye.y };
	}
	else {
		float eyeX = MathTool::clamp(_eye.x + _Z_DISTANCE, _2D_MIN_X, _2D_MAX_X);
		return { eyeX, _eye.y - 5.0f };
	}
}

Float3& Camera::getPos() {
	return _eye;
}

void CameraController2D::move(Float3 targetPos, Float3& eye, Float3& focus) {
	eye.x = targetPos.x;
	eye.x = MathTool::clamp(eye.x, Camera::_2D_MIN_X, Camera::_2D_MAX_X);
	focus.x = eye.x;

	eye.y = targetPos.y;
	eye.y = MathTool::clamp(eye.y, 0.0f, Camera::_MAX_Y);
	focus.y = eye.y;

	eye.z = -Camera::_Z_DISTANCE;
}

void CameraController3D::move(Float3 targetPos, Float3& eye, Float3& focus) {
	focus.x = targetPos.x;
	focus.x = MathTool::clamp(focus.x, Camera::_3D_MIN_X + Camera::_Z_DISTANCE, Camera::_3D_MAX_X + Camera::_Z_DISTANCE);
	eye.x = focus.x - Camera::_Z_DISTANCE;

	focus.y = targetPos.y;
	focus.y = MathTool::clamp(focus.y, 0.0f, Camera::_MAX_Y);
	eye.y = focus.y + 5.0f;

	eye.z = 0.0f;
}

void CameraController2D::transform(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::clamp(PLAYER.getPos().x, Camera::_2D_MIN_X, Camera::_2D_MAX_X);
	const float destEyePosX = MathTool::clamp(PLAYER.getPos().x - Camera::_Z_DISTANCE, Camera::_3D_MIN_X, Camera::_3D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.y += 5.0f / 30.0f;
	eye.z += Camera::_Z_DISTANCE / 30.0f;

	const float srcFocusPosX = MathTool::clamp(PLAYER.getPos().x, Camera::_2D_MIN_X, Camera::_2D_MAX_X);
	const float destFocusPosX = MathTool::clamp(PLAYER.getPos().x, Camera::_3D_MIN_X + Camera::_Z_DISTANCE, Camera::_3D_MAX_X + Camera::_Z_DISTANCE);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeInQuad<XMMATRIX>(ortho, perspective, step);
}

void CameraController3D::transform(Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::clamp(PLAYER.getPos().x - Camera::_Z_DISTANCE, Camera::_3D_MIN_X, Camera::_3D_MAX_X);
	const float destEyePosX = MathTool::clamp(PLAYER.getPos().x, Camera::_2D_MIN_X, Camera::_2D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.y -= 5.0f / 30.0f;
	eye.z -= Camera::_Z_DISTANCE / 30.0f;

	const float srcFocusPosX = MathTool::clamp(PLAYER.getPos().x, Camera::_3D_MIN_X + Camera::_Z_DISTANCE, Camera::_3D_MAX_X + Camera::_Z_DISTANCE);
	const float destFocusPosX = MathTool::clamp(PLAYER.getPos().x, Camera::_2D_MIN_X, Camera::_2D_MAX_X);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeOutQuad<XMMATRIX>(perspective, ortho, step);
}