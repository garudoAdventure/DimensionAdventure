#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "./Common/MathStruct.h"

enum class VS {
	GENERAL,
};

enum class PS {
	GENERAL,
	NO_TEX,
	LUMINANCE,
	BLUR,
	BLOOM,
	GLITCH,
	NOISE,
	SPIRAL
};

enum class SamplerState {
	WRAP,
	CLAMP
};

class Shader {
  public:
		void begin();
		void setVS(VS vs);
		void setPS(PS ps);
		void setSamplerState(SamplerState state);
		void setWorld(XMMATRIX& world);
		void setView(Float3 e, Float3 f);
		void set2DMatrix();
		void setMatrix();
		void setMatrix(Transpose& mat);
		void setProjection(XMMATRIX projMatrix);
		void setLight(const Light light);
		XMMATRIX getOrthoMatrix();
		XMMATRIX getPerspectiveMatrix();
		XMMATRIX getInverseView();

  private:
		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _deviceContext = nullptr;
		ID3D11VertexShader* _vertexShader = nullptr;
		ID3D11PixelShader* _pixelShader = nullptr;
		ID3D11PixelShader* _noTexShader = nullptr;
		ID3D11PixelShader* _luminanceShader = nullptr;
		ID3D11PixelShader* _blurShader = nullptr;
		ID3D11PixelShader* _bloomShader = nullptr;
		ID3D11PixelShader* _glitchShader = nullptr;
		ID3D11PixelShader* _noiseShader = nullptr;
		ID3D11PixelShader* _spiralShader = nullptr;
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