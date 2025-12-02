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

class LayerScreen {
	public:
		LayerScreen() {
			layerTex[0] = new RenderTexture(1280.0f, 720.0f);
			layerTex[1] = new RenderTexture(1280.0f, 720.0f);
			layerTex[2] = new RenderTexture(1280.0f, 720.0f);
		}
		void update(Camera* camera, Field* currentField) {
			layerNum = PLAYER.getCrystalNum();

			camera->draw();
			for (int i = 0; i < 3; i++) {
				layerTex[i]->setTargetView();
				layerTex[i]->clear();
				currentField->draw(i);
				PLAYER.draw();
			}
			//playerLayerTex.setTargetView();
			//playerLayerTex.clear();
			//player->draw();
		}
		void draw() {
			XMVECTOR eye = XMVectorSet(_eye.x, _eye.y, _eye.z, 0.0f);
			XMVECTOR focus = XMVectorSet(0.0f, 0.0f, 22.5f, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMMATRIX view = XMMatrixLookAtLH(eye, focus, up);
			SHADER.begin();
			SHADER.set3DMatrix(view);
			if (startEffect) {
				TEXTURE.setTexture(layerTex[layerIdx]->getTex());
				SPRITE.drawTextureSprite({ 0.0f, 0.0f, _posZ }, { 1280.0f, 720.0f }, 1.0f);

				float alpha = MathTool::lerp<float>(1.0f, 0.0f, startEffectCount / 15.0f);
				float width = MathTool::lerp<float>(1280.0f, 2560.0f, startEffectCount / 15.0f);
				float height = MathTool::lerp<float>(720.0f, 1440.0f, startEffectCount / 15.0f);
				TEXTURE.setTexture(layerTex[layerIdx]->getTex());
				SPRITE.drawTextureSprite({ 0.0f, 0.0f, _posZ - 1.0f }, { width, height }, alpha);
				return;
			}
			for (int i = layerNum; i >= -1; i--) {
				int idx = (i + layerIdx + layerNum) % layerNum;
				float alpha = 1.0f;
				if (i == -1)					 alpha = 1 - alphaBack;
				if (i == 0)						 alpha = alphaFront;
				if (i == layerNum - 1) alpha = alphaBack;
				if (i == layerNum)		 alpha = 1 - alphaFront;
				TEXTURE.setTexture(layerTex[idx]->getTex());
				SPRITE.drawTextureSprite({
					0.0f, _posY + offsetY * i, _posZ + offsetZ * i
				}, { 1280.0f, 720.0f }, alpha);
			}
			//TEXTURE.setTexture(playerLayerTex.getTex());
			//SPRITE.drawTextureSprite({ 0.0, 0.0f, 2.4f }, { 1280.0f, 720.0f }, 1.0f);
		}
		int getCurrentLayer() {
			return currentLayer;
		}
		bool selectLayer() {
			if (startEffect) {
				if (startEffectCount == 15) {
					startEffect = false;
				}
				else {
					startEffectCount++;
				}
				return true;
			}
			if (transformIn()) {
				return true;
			}
			if (_startTransformOut) {
				if (!transformOut()) {
					_startTransformOut = false;
					transformInCount = 1;
					transformOutCount = 1;
					startEffect = true;
					startEffectCount = 0;
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
				oldLayerIdx += 1;
				if (oldLayerIdx > layerNum - 1) {
					oldLayerIdx -= layerNum;
				}
			}
			if (Keyboard_IsKeyTrigger(KK_LEFT)) {
				_isSelectBack = true;
				oldLayerIdx -= 1;
				if (oldLayerIdx < 0) {
					oldLayerIdx += layerNum;
				}
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				currentLayer = layerIdx;
				_startTransformOut = true;
			}
			return true;
		}

	private:
		RenderTexture* layerTex[3];
		// RenderTexture* playerLayerTex;
		Float3 _eye = { 0.0f, 0.0f, -30.0f };
		const float offsetY = 80.0f;
		float _posY = 0.0f;
		const float offsetZ = 20.0f;
		float _posZ = 2.5f;
		int layerNum = 0;
		int oldLayerIdx = 0;
		int layerIdx = 0;
		int currentLayer = 0;
		int transformInCount = 1;
		int transformOutCount = 1;
		int selectFadeCount = 1;
		float alphaFront = 1.0f;
		float alphaBack = 1.0f;
		bool _startTransformOut = false;
		bool _isSelectFront = false;
		bool _isSelectBack = false;
		bool startEffect = true;
		int startEffectCount = 0;

		bool transformIn() {
			if (transformInCount <= 30) {
				_eye.x = MathTool::lerp<float>(0.0f, -35.0f, transformInCount / 30.0f);
				_eye.y = MathTool::lerp<float>(0.0f, 14.0f, transformInCount / 30.0f);
				_eye.z = MathTool::lerp<float>(-30.0f, -45.0f, transformInCount / 30.0f);
				transformInCount++;
				return true;
			}
			return false;
		}

		bool transformOut() {
			if (transformOutCount <= 30) {
				_eye.x = MathTool::lerp<float>(-35.0f, 0.0f, transformOutCount / 30.0f);
				_eye.y = MathTool::lerp<float>(14.0f, 0.0f, transformOutCount / 30.0f);
				_eye.z = MathTool::lerp<float>(-45.0f, -30.0f, transformOutCount / 30.0f);
				transformOutCount++;
				return true;
			}
			return false;
		}

		bool selectFrontFade() {
			if (oldLayerIdx == layerIdx) return false;
			if (selectFadeCount < 10) {
				if (_isSelectFront) {
					_posY = MathTool::lerp<float>(0.0f, 0.0f - offsetY, (selectFadeCount + 1) / 10.0f);
					_posZ = MathTool::lerp<float>(2.5f, 2.5f - offsetZ, (selectFadeCount + 1) / 10.0f);
					alphaFront = MathTool::lerp<float>(1.0f, 0.0f, (selectFadeCount + 1) / 10.0f);
				}
				if (_isSelectBack) {
					_posY = MathTool::lerp<float>(0.0f, 0.0f + offsetY, (selectFadeCount + 1) / 10.0f);
					_posZ = MathTool::lerp<float>(2.5f, 2.5f + offsetZ, (selectFadeCount + 1) / 10.0f);
					alphaBack = MathTool::lerp<float>(1.0f, 0.0f, (selectFadeCount + 1) / 10.0f);
				}
				selectFadeCount++;
				return true;
			}
			_posY = 0.0f;
			_posZ = 2.5f;
			layerIdx = oldLayerIdx;
			alphaFront = 1.0f;
			alphaBack = 1.0f;
			selectFadeCount = 0;
			return false;
		}
};