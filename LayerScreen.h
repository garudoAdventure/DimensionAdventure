#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "RenderTexture.h"
#include "Camera.h"
#include "Field.h"
#include "Player.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "PostProcess.h"
#include "SystemDialog.h"
#include "IGameEventHandler.h"
#include "SpaceBG.h"

class LayerScreen {
public:
	static constexpr int ALL_LAYER_NUM = 4;

	LayerScreen(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
		Float4 layerColor[4] = { Color::lightRed, Color::lightGreen, Color::lightBlue, Color::white };
		const unsigned int frameTex = TEXTURE.loadTexture("./assets/UI/frame.png");
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			_layerTex[i] = new RenderTexture(1280.0f, 720.0f, Color::darkGray);
			RenderTexture* frameEffectTex = new RenderTexture(1280.0f, 720.0f);
			frameEffectTex->setTargetView();
			frameEffectTex->clear();
			SHADER.begin();
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1267.0f, 713.0f }, TEXTURE.getTexture(frameTex), layerColor[i]);

			_postProcess[i] = new PostProcess(frameEffectTex);
			_postProcess[i]->update();
		}
		_dialog = new SystemDialog({ L"ホワイトレイヤーを発見した！" });
	}

	~LayerScreen() {
		delete _dialog;
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			delete _layerTex[i];
			delete _postProcess[i];
		}
	}

	void update() {
		checkWhiteLayerCommand();
		_layerNum = PLAYER.getCrystalNum() + (_isWhiteLayerActive ? 1 : 0);
		bg.update();
	}

	void drawScreen(Camera* camera, Field* currentField) {
		camera->draw();
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			_gameEvent->drawOffscreen((i + 1) % 4);

			_layerTex[i]->setTargetView();
			_layerTex[i]->clear();
			_gameEvent->drawGameScene((i + 1) % 4);
			_postProcess[i]->drawBloom();
		}
	}

	void draw() {
		Float4 white = Color::white;
		SHADER.begin();
		if (_startEffect) {
			XMMATRIX world = XMMatrixIdentity();
			world *= XMMatrixTranslation(0.0f, 0.0f, _posZ);
			SHADER.setWorld(world);
			SHADER.setView(_eye, { 0.0f, 0.0f, 22.5f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 1280.0f, 720.0f }, _layerTex[_layerIdx]->getTex(), Color::white);

			float alpha = MathTool::lerp<float>(1.0f, 0.0f, _startEffectCount / 15.0f);
			float width = MathTool::lerp<float>(1280.0f, 2560.0f, _startEffectCount / 15.0f);
			float height = MathTool::lerp<float>(720.0f, 1440.0f, _startEffectCount / 15.0f);
			world = XMMatrixIdentity();
			world *= XMMatrixTranslation(0.0f, 0.0f, _posZ - 1.0f);
			SHADER.setWorld(world);
			SHADER.setView(_eye, { 0.0f, 0.0f, 22.5f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			SHADER.setMatrix();
			white.a = alpha;
			SPRITE.drawSprite3D({ width, height }, _layerTex[_layerIdx]->getTex(), white);
			return;
		}

		bg.draw();

		for (int i = _layerNum; i >= -1; i--) {
			int idx = (i + _layerIdx + _layerNum) % _layerNum;
			float alpha = 1.0f;
			if (i == -1)					 alpha = 1 - _alphaBack;
			if (i == 0)						 alpha = _alphaFront;
			if (i == _layerNum - 1) alpha = _alphaBack;
			if (i == _layerNum)		 alpha = 1 - _alphaFront;

			XMMATRIX world = XMMatrixIdentity();
			world *= XMMatrixTranslation(0.0f, _posY + _offsetY * i, _posZ + _offsetZ * i);
			SHADER.setWorld(world);
			SHADER.setView(_eye, { 0.0f, 0.0f, 22.5f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			SHADER.setMatrix();
			white.a = alpha;
			SPRITE.drawSprite3D({ 1280.0f, 720.0f }, _layerTex[idx]->getTex(), white);
		}
		if (_isWhiteLayerActive && !_isFindWhiteLayer) {
			_dialog->draw();
		}
	}

	int getCurrentLayer() {
		return _currentLayer;
	}

	bool selectLayer() {
		if (_startEffect) {
			if (_startEffectCount == 15) {
				_startEffect = false;
			}
			else {
				_startEffectCount++;
			}
			return true;
		}

		if (transformIn()) {
			return true;
		}

		if (_startTransformOut) {
			if (!transformOut()) {
				_startTransformOut = false;
				_transformInCount = 1;
				_transformOutCount = 1;
				_startEffect = true;
				_startEffectCount = 0;
				return false;
			}
			return true;
		}

		if (selectFrontFade()) {
			return true;
		}

		_isSelectFront = false;
		_isSelectBack = false;
		if (Keyboard_IsKeyTrigger(KK_RIGHT)) {
			_isSelectFront = true;
			_oldLayerIdx += 1;
			if (_oldLayerIdx > _layerNum - 1) {
				_oldLayerIdx -= _layerNum;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_LEFT)) {
			_isSelectBack = true;
			_oldLayerIdx -= 1;
			if (_oldLayerIdx < 0) {
				_oldLayerIdx += _layerNum;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_ENTER)) {
			_currentLayer = (_layerIdx + 1) % 4;
			_startTransformOut = true;
			_correctCommandNum = 0;
		}
		return true;
	}

private:
	IGameEventHandler* _gameEvent;
	RenderTexture* _layerTex[4];
	PostProcess* _postProcess[4];
	SpaceBG bg;
	IDialog* _dialog;
	Float3 _eye = { 0.0f, 0.0f, -30.0f };
	float _posY = 0.0f;
	float _posZ = 2.5f;
	const float _offsetY = 3.0f;
	const float _offsetZ = 10.0f;
	int _layerNum = 0;
	int _oldLayerIdx = 0;
	int _layerIdx = 0;
	int _currentLayer = 0;
	int _transformInCount = 1;
	int _transformOutCount = 1;
	int _selectFadeCount = 1;
	float _alphaFront = 1.0f;
	float _alphaBack = 1.0f;
	bool _startTransformOut = false;
	bool _isSelectFront = false;
	bool _isSelectBack = false;
	bool _startEffect = true;
	int _startEffectCount = 0;
	bool _isWhiteLayerActive = false;
	int _correctCommandNum = 0;
	bool _isFindWhiteLayer = false;
	int _showDialogCount = 0;

	bool transformIn() {
		if (_transformInCount <= 30) {
			_eye.x = MathTool::lerp<float>(0.0f, -35.0f, _transformInCount / 30.0f);
			_eye.y = MathTool::lerp<float>(0.0f, 14.0f, _transformInCount / 30.0f);
			_eye.z = MathTool::lerp<float>(-30.0f, -45.0f, _transformInCount / 30.0f);
			_transformInCount++;
			return true;
		}
		return false;
	}

	bool transformOut() {
		if (_transformOutCount <= 30) {
			_eye.x = MathTool::lerp<float>(-35.0f, 0.0f, _transformOutCount / 30.0f);
			_eye.y = MathTool::lerp<float>(14.0f, 0.0f, _transformOutCount / 30.0f);
			_eye.z = MathTool::lerp<float>(-45.0f, -30.0f, _transformOutCount / 30.0f);
			_transformOutCount++;
			return true;
		}
		return false;
	}

	bool selectFrontFade() {
		if (_oldLayerIdx == _layerIdx) return false;
		if (_selectFadeCount < 10) {
			if (_isSelectFront) {
				_posY = MathTool::lerp<float>(0.0f, 0.0f - _offsetY, (_selectFadeCount + 1) / 10.0f);
				_posZ = MathTool::lerp<float>(2.5f, 2.5f - _offsetZ, (_selectFadeCount + 1) / 10.0f);
				_alphaFront = MathTool::lerp<float>(1.0f, 0.0f, (_selectFadeCount + 1) / 10.0f);
			}
			if (_isSelectBack) {
				_posY = MathTool::lerp<float>(0.0f, 0.0f + _offsetY, (_selectFadeCount + 1) / 10.0f);
				_posZ = MathTool::lerp<float>(2.5f, 2.5f + _offsetZ, (_selectFadeCount + 1) / 10.0f);
				_alphaBack = MathTool::lerp<float>(1.0f, 0.0f, (_selectFadeCount + 1) / 10.0f);
			}
			_selectFadeCount++;
			return true;
		}
		_posY = 0.0f;
		_posZ = 2.5f;
		_layerIdx = _oldLayerIdx;
		_alphaFront = 1.0f;
		_alphaBack = 1.0f;
		_selectFadeCount = 0;
		return false;
	}

	void checkWhiteLayerCommand() {
		if (_isWhiteLayerActive || _layerNum < 3) {
			if (_isWhiteLayerActive && !_isFindWhiteLayer) {
				_dialog->update();
				_showDialogCount++;
				if (_showDialogCount == 180) {
					_isFindWhiteLayer = true;
				}
			}
			return;
		}
		if (Keyboard_IsKeyTrigger(KK_UP)) {
			if (_correctCommandNum == 0) {
				_correctCommandNum = 1;
				return;
			}
			if (_correctCommandNum == 2) {
				_correctCommandNum = 3;
				return;
			}
			if (_correctCommandNum == 5) {
				_isWhiteLayerActive = true;
				_gameEvent->setCheckpoint(CheckPoint::WHITE_LAYER);
				return;
			}
			_correctCommandNum = 0;
		}
		if (Keyboard_IsKeyTrigger(KK_DOWN)) {
			if (_correctCommandNum == 1) {
				_correctCommandNum = 2;
				return;
			}
			if (_correctCommandNum == 3) {
				_correctCommandNum = 4;
				return;
			}
			if (_correctCommandNum == 4) {
				_correctCommandNum = 5;
				return;
			}
			_correctCommandNum = 0;
		}
	}
};