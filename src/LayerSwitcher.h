#pragma once

#include "Camera.h"
#include "HALKeyboard.h"
#include "RenderTexture.h"
#include "SwitchLayerBg.h"
#include "Sound.h"
#include "./Game/IGameEventHandler.h"
#include "./Field/Field.h"
#include "./Dialog/SystemDialog.h"
#include "./Player/Player.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./PostProcess/Bloom.h"
#include "./Utils/MathTool.h"

class LayerSwitcher {
public:
	static constexpr int ALL_LAYER_NUM = 4;
	static constexpr int START_ANIMATION_TIME = 15;
	static constexpr int MOVE_EYE_TIME = 30;
	static constexpr int LAYER_FADE_ANIMATION_TIME = 10;

	LayerSwitcher(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
		const unsigned int border = TEXTURE.loadTexture("./assets/UI/border.png");
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			_layerTex[i] = new RenderTexture(1280.0f, 720.0f, Color::darkGray);
			RenderTexture* borderTex = new RenderTexture(1280.0f, 720.0f);
			borderTex->setTargetView();
			borderTex->clear();
			SHADER.begin();
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1267.0f, 713.0f }, TEXTURE.getTexture(border), Color::layerColor[(i + 1) % 4]);

			_bloomBorder[i] = new Bloom(borderTex);
			_bloomBorder[i]->setClipLuminance(false);
			_bloomBorder[i]->update();
		}
		_cameraEye = _fixedCamera[0];
		_bg = new SwitchLayerBg();
		_dialog = new SystemDialog({ L"ホワイトレイヤーを発見した！" });
		_changeLayerSE = SOUND.loadSound("./assets/sound/changeLayer.wav");
		_confirmLayerSE = SOUND.loadSound("./assets/sound/confirmLayer.wav");
		_findWhiteLayerSE = SOUND.loadSound("./assets/sound/findWhiteLayer.wav");
	}

	~LayerSwitcher() {
		delete _dialog;
		delete _bg;
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			delete _layerTex[i];
			delete _bloomBorder[i];
		}
	}

	void init() {
		_isStartSwitchLayerAnimation = true;
		_startSwitchLayerAnimationCount = 0;
		_isConfirmLayer = false;
		_isSelectLayerEnd = false;
	}

	void update() {
		checkWhiteLayerCommand();
		_layerNum = PLAYER.getCrystalNum() + (_isWhiteLayerActive ? 1 : 0);
		_bg->update();

		if (_isStartSwitchLayerAnimation) {
			if (_startSwitchLayerAnimationCount < START_ANIMATION_TIME) {
				_startSwitchLayerAnimationCount++;
			}
			else {
				_isStartSwitchLayerAnimation = false;
				_oldCameraEye = _cameraEye;
				_cameraIdx = 1;
				_moveEyeCount = 0;
			}
			return;
		}

		if (moveCameraEye()) {
			return;
		}

		if (_isConfirmLayer) {
			_isSelectLayerEnd = true;
			SOUND.setVolume(_gameEvent->getBgmId(), 0.5f);
			return;
		}

		if (switchLayerAnimation()) {
			return;
		}

		_isSelectFront = false;
		_isSelectBack = false;

		if (Keyboard_IsKeyTrigger(KK_RIGHT)) {
			SOUND.playSound(_changeLayerSE, 0);
			_isSelectFront = true;
			_oldLayerIdx += 1;
			if (_oldLayerIdx > _layerNum - 1) {
				_oldLayerIdx -= _layerNum;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_LEFT)) {
			SOUND.playSound(_changeLayerSE, 0);
			_isSelectBack = true;
			_oldLayerIdx -= 1;
			if (_oldLayerIdx < 0) {
				_oldLayerIdx += _layerNum;
			}
		}
		if (Keyboard_IsKeyTrigger(KK_ENTER)) {
			_isConfirmLayer = true;
			SOUND.playSound(_confirmLayerSE, 0);
			_currentLayer = (_layerIdx + 1) % 4;
			_correctCommandNum = 0;
			_oldCameraEye = _cameraEye;
			_cameraIdx = 0;
			_moveEyeCount = 0;
		}
	}

	void drawGameSceneOnLayer(Camera* camera, Field* currentField) {
		camera->draw();
		for (int i = 0; i < ALL_LAYER_NUM; i++) {
			_gameEvent->drawOffscreen((i + 1) % 4);

			_layerTex[i]->setTargetView();
			_layerTex[i]->clear();
			_gameEvent->drawGameScene((i + 1) % 4);
			_bloomBorder[i]->drawBloom();
		}
	}

	void draw() {
		Float4 white = Color::white;
		SHADER.begin();
		if (_isStartSwitchLayerAnimation) {
			SHADER.set2DMatrix();
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _layerTex[_layerIdx]->getTex(), Color::white);

			float width = MathTool::lerp<float>(1280.0f, 1280.0f * 1.5f, (float)_startSwitchLayerAnimationCount / START_ANIMATION_TIME);
			float height = MathTool::lerp<float>(720.0f, 720.0f * 1.5f, (float)_startSwitchLayerAnimationCount / START_ANIMATION_TIME);
			white.a = MathTool::lerp<float>(1.0f, 0.5f, (float)_startSwitchLayerAnimationCount / START_ANIMATION_TIME);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { width, height }, _layerTex[_layerIdx]->getTex(), white);
			
			return;
		}

		_bg->draw();

		for (int i = _layerNum; i >= -1; i--) {
			int idx = (i + _layerIdx + _layerNum) % _layerNum;
			float alpha = 1.0f;
			if (i == -1)					  alpha = 1 - _alphaBack;
			if (i == 0)						  alpha = _alphaFront;
			if (i == _layerNum - 1) alpha = _alphaBack;
			if (i == _layerNum)		  alpha = 1 - _alphaFront;

			Float3 layerPos = _lastLayerPos + _layerOffset * i;
			XMMATRIX world = XMMatrixIdentity();
			world *= XMMatrixTranslation(layerPos.x, layerPos.y, layerPos.z);
			SHADER.setWorld(world);
			SHADER.setView(_cameraEye, { 0.0f, 0.0f, 22.5f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			SHADER.setMatrix();
			white.a = alpha;
			SPRITE.drawSprite3D({ 1280.0f, 720.0f }, _layerTex[idx]->getTex(), white);
		}
		if (_isWhiteLayerActive && !_isFindWhiteLayer) {
			if (!_dialog->isEnd()) {
				_dialog->draw();
			}
		}
	}

	int getCurrentLayer() {
		return _currentLayer;
	}

	bool isSelectLayerEnd() {
		return _isSelectLayerEnd;
	}

private:
	unsigned int _changeLayerSE;
	unsigned int _confirmLayerSE;
	unsigned int _findWhiteLayerSE;

	IGameEventHandler* _gameEvent;
	RenderTexture* _layerTex[4];
	Bloom* _bloomBorder[4];
	SwitchLayerBg* _bg;
	IDialog* _dialog;
	
	Float3 _cameraEye;
	Float3 _oldCameraEye;
	int _moveEyeCount = 0;
	int _cameraIdx = 0;
	const Float3 _fixedCamera[2] = {
		{ 0.0f, 0.0f, -30.0f },
		{ -35.0f, 14.0f, -45.0f }
	};

	Float3 _initLastLayerPos { 0.0f, 0.0f, 2.5f };
	Float3 _lastLayerPos = _initLastLayerPos;
	Float3 _layerOffset{ 0.0f, 3.0f, 10.0f };
	
	int _layerNum = 0;
	int _oldLayerIdx = 0;
	int _layerIdx = 0;
	int _currentLayer = 0;
	
	int _selectFadeCount = 1;
	float _alphaFront = 1.0f;
	float _alphaBack = 1.0f;
	bool _isConfirmLayer = false;
	bool _isSelectLayerEnd = false;
	bool _isSelectFront = false;
	bool _isSelectBack = false;
	
	bool _isStartSwitchLayerAnimation = true;
	int _startSwitchLayerAnimationCount = 0;
	
	bool _isWhiteLayerActive = false;
	int _correctCommandNum = 0;
	bool _isFindWhiteLayer = false;
	int _showDialogCount = 0;
	enum KeyCommand {
		NONE,
		UP,
		DOWN
	};
	const KeyCommand whiteLayerCommand[6] = {
		UP, DOWN, UP, DOWN, DOWN, UP
	};

	bool moveCameraEye() {
		_cameraEye = MathTool::lerp<Float3>(_oldCameraEye, _fixedCamera[_cameraIdx], (float)_moveEyeCount / MOVE_EYE_TIME);
		_moveEyeCount = std::min(MOVE_EYE_TIME, _moveEyeCount + 1);
		return _moveEyeCount != MOVE_EYE_TIME;
	}

	bool switchLayerAnimation() {
		if (_oldLayerIdx == _layerIdx) return false;
		if (_selectFadeCount < LAYER_FADE_ANIMATION_TIME) {
			if (_isSelectFront) {
				_lastLayerPos = MathTool::lerp<Float3>(
					_initLastLayerPos, _initLastLayerPos - _layerOffset, (float)(_selectFadeCount + 1) / LAYER_FADE_ANIMATION_TIME
				);
				_alphaFront = MathTool::lerp<float>(1.0f, 0.0f, (float)(_selectFadeCount + 1) / LAYER_FADE_ANIMATION_TIME);
			}
			if (_isSelectBack) {
				_lastLayerPos = MathTool::lerp<Float3>(
					_initLastLayerPos, _initLastLayerPos + _layerOffset, (float)(_selectFadeCount + 1) / LAYER_FADE_ANIMATION_TIME
				);
				_alphaBack = MathTool::lerp<float>(1.0f, 0.0f, (float)(_selectFadeCount + 1) / LAYER_FADE_ANIMATION_TIME);
			}
			_selectFadeCount++;
			return true;
		}
		_lastLayerPos = _initLastLayerPos;
		_layerIdx = _oldLayerIdx;
		_alphaFront = _alphaBack = 1.0f;
		_selectFadeCount = 0;
		return false;
	}

	void checkWhiteLayerCommand() {
		static int currectIdx = 0;
		if (_isWhiteLayerActive || _layerNum < 3) {
			if (_isWhiteLayerActive && !_isFindWhiteLayer) {
				_dialog->update();
				_showDialogCount++;
				if (_showDialogCount == 120) {
					_isFindWhiteLayer = true;
				}
			}
			return;
		}
		KeyCommand command = KeyCommand::NONE;
		if (Keyboard_IsKeyTrigger(KK_UP)) {
			command = KeyCommand::UP;
		}
		if (Keyboard_IsKeyTrigger(KK_DOWN)) {
			command = KeyCommand::DOWN;
		}

		if (command == KeyCommand::NONE) {
			return;
		}
		if (command != whiteLayerCommand[currectIdx]) {
			currectIdx = 0;
		}
		else {
			currectIdx += 1;
			if (currectIdx == 6) {
				_isWhiteLayerActive = true;
				_gameEvent->setCheckpoint(CheckPoint::WHITE_LAYER);
				SOUND.playSound(_findWhiteLayerSE, 0);
			}
		}
	}
};