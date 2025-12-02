#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "MathStruct.h"

enum class PS {
	NORMAL,
	BLUR,
	LUMINANCE
};

enum class SamplerState {
	WRAP,
	CLAMP
};

class Shader {
  public:
		void begin();
		void setPS(PS ps);
		void setSamplerState(SamplerState state);
		void setWorldMatrix(XMMATRIX& world);
		void setWorldMatrix(Float3 pos, Float3 rotate = { 0.0f, 0.0f, 0.0f }, Float3 scale = { 1.0f, 1.0f, 1.0f });
		void set2DMatrix();
		void set3DMatrix(XMMATRIX view);
		void setMatrix();
		void setMatrix(Transpose& mat);
		void setView(Float3 e, Float3 f);
		void setProjection(XMMATRIX projMatrix);
		void setLight(const Light light);
		XMMATRIX getOrthoMatrix();
		XMMATRIX getPerspectiveMatrix();

  private:
		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _deviceContext = nullptr;
		ID3D11VertexShader* _vertexShader = nullptr;
		ID3D11PixelShader* _pixelShader = nullptr;
		ID3D11PixelShader* _blurShader = nullptr;
		ID3D11PixelShader* _luminanceShader = nullptr;
		ID3D11InputLayout* _inputLayout = nullptr;
		ID3D11Buffer* _matrixBuffer = nullptr;
		ID3D11Buffer* _lightBuffer = nullptr;
		ID3D11SamplerState* _samplerWrapState = nullptr;
		ID3D11SamplerState* _samplerClampState = nullptr;
		XMMATRIX _orthoMatrix;
		XMMATRIX _perspectiveMatrix;
		XMMATRIX _worldMatrix;
		XMMATRIX _viewMatrix;
		XMMATRIX _projMatrix;

  public:
		static void CreateInstance(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
			DeleteInstance();
			s_instance = new Shader(device, deviceContext);
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static Shader& GetInstance() {
			return *s_instance;
		}

  private:
		Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		~Shader();
		static inline Shader* s_instance;
};

#define SHADER Shader::GetInstance()