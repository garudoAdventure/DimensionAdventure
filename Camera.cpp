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

void Camera::transformDimension(Player* player) {
	if (!_isTransforming) {
		count = 1;
		_isTransforming = true;
		return;
	}
	const int maxCount = 30;
	float step = (float)count / maxCount;

	cameraController->rotate(player, _eye, _focus, projMatrix, step);

	if (count < maxCount) {
		count++;
	}
	else {
		_is2DMode = !_is2DMode;
		_isTransforming = false;
		cameraController = _is2DMode ? cameraController2D : cameraController3D;
	}
}

void Camera::moveCamera(Player* player) {
	cameraController->move(player, _newEye, _newFocus);
	_eye = MathTool::lerp<Float3>(_eye, _newEye, 0.1f);
	_focus = MathTool::lerp<Float3>(_focus, _newFocus, 0.1f);
}

void Camera::set2DPos(Float2 pos) {
	_eye.x = pos.x;
	_eye.x = MathTool::getInRange(_eye.x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	_focus.x = _eye.x;
	_newEye.x = _eye.x;

	_eye.y = pos.y;
	_eye.y = MathTool::getInRange(_eye.y, 0.0f, 20.0f);
	_focus.y = _eye.y;
	_newEye.y = _eye.y;
}

Float2 Camera::get2DPos() {
	if (_is2DMode) {
		return { _eye.x, _eye.y };
	}
	else {
		float eyeX = MathTool::getInRange(_eye.x + 30.0f, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
		return { eyeX, _eye.y };
	}
}

void CameraController2D::move(Player* player, Float3& eye, Float3& focus) {
	eye.x = player->getPos().x;
	eye.x = MathTool::getInRange(eye.x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	focus.x = eye.x;

	eye.y = player->getPos().y;
	eye.y = MathTool::getInRange(eye.y, 0.0f, 20.0f);
	focus.y = eye.y;

	eye.z = -30;
}

void CameraController3D::move(Player* player, Float3& eye, Float3& focus) {
	focus.x = player->getPos().x;
	focus.x = MathTool::getInRange(focus.x, CAMERA_3D_MIN_X + 30.0f, CAMERA_3D_MAX_X + 30.0f);
	eye.x = focus.x - 30.0f;

	focus.y = player->getPos().y + 5.0f;
	eye.y = focus.y;

	eye.z = 0.0f;
}

void CameraController2D::rotate(Player* player, Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::getInRange(player->getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	const float destEyePosX = MathTool::getInRange(player->getPos().x - 30.0f, CAMERA_3D_MIN_X, CAMERA_3D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.z += 1.0f;

	const float srcFocusPosX = MathTool::getInRange(player->getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	const float destFocusPosX = MathTool::getInRange(player->getPos().x, CAMERA_3D_MIN_X + 30.0f, CAMERA_3D_MAX_X + 30.0f);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeInQuad<XMMATRIX>(ortho, perspective, step);
}

void CameraController3D::rotate(Player* player, Float3& eye, Float3& focus, XMMATRIX& projMat, float step) {
	const float srcEyePosX = MathTool::getInRange(player->getPos().x - 30.0f, CAMERA_3D_MIN_X, CAMERA_3D_MAX_X);
	const float destEyePosX = MathTool::getInRange(player->getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	eye.x = MathTool::lerp(srcEyePosX, destEyePosX, step);
	eye.z -= 1.0f;

	const float srcFocusPosX = MathTool::getInRange(player->getPos().x, CAMERA_3D_MIN_X + 30.0f, CAMERA_3D_MAX_X + 30.0f);
	const float destFocusPosX = MathTool::getInRange(player->getPos().x, CAMERA_2D_MIN_X, CAMERA_2D_MAX_X);
	focus.x = MathTool::lerp(srcFocusPosX, destFocusPosX, step);

	XMMATRIX ortho = SHADER.getOrthoMatrix();
	XMMATRIX perspective = SHADER.getPerspectiveMatrix();
	projMat = MathTool::easeOutQuad<XMMATRIX>(perspective, ortho, step);
}