#pragma once

#include "RenderTexture.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"

class ColorGrading {
	public:
		ColorGrading(RenderTexture* tex) : _texture(tex) {
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(ColorSetting);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &_colorBuffer);
		}

		void draw(int layerIdx) {
			SHADER.setPS(PS::COLOR_GRADING);

			ColorSetting cs;
			switch (layerIdx) {
				case 1:
					cs.saturation = 1.35f;
					cs.whiteBalance = getWhiteBalance(0.2f, 0.1f);
					break;
				case 2:
					cs.saturation = 1.3f;
					cs.whiteBalance = getWhiteBalance(-0.1f, -0.2f);
					break;
				case 3:
					cs.saturation = 1.35f;
					cs.whiteBalance = getWhiteBalance(-0.2f, -0.1f);
					break;
			}
			
			DX3D.getDeviceContext()->UpdateSubresource(_colorBuffer, 0, NULL, &cs, 0, 0);
			DX3D.getDeviceContext()->PSSetConstantBuffers(2, 1, &_colorBuffer);

			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _texture->getTex(), Color::white);

			SHADER.setPS(PS::GENERAL);
		}

	private:
		RenderTexture* _texture;
		ID3D11Buffer* _colorBuffer;

		float StandardIlluminantY(float x) {
			return 2.87f * x - 3.0f * x * x - 0.27509507f;
		}

		Float3 CIExyToLMS(float x, float y) {
			float Y = 1.0f;
			float X = Y * x / y;
			float Z = Y * (1.0f - x - y) / y;

			float L = 0.7328f * X + 0.4296f * Y - 0.1624f * Z;
			float M = -0.7036f * X + 1.6975f * Y + 0.0061f * Z;
			float S = 0.0030f * X + 0.0136f * Y + 0.9834f * Z;

			return { L, M, S };
		}
		
		XMFLOAT3 getWhiteBalance(float temp, float tint) {
			float t1 = temp / 0.65;
			float t2 = tint / 0.65;
			float x = 0.31271 - t1 * (t1 < 0 ? 0.1 : 0.05f);
			float y = StandardIlluminantY(x) + t2 * 0.05;
			
			Float3 w1 = { 0.94923f, 1.03542f, 1.08728f };
			Float3 w2 = CIExyToLMS(x, y);

			return { w1.x / w2.x, w1.y / w2.y, w1.z / w2.z };
		}
};