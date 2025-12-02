#pragma once

#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#define SAFE_RELEASE(o)	  if(o) { (o)->Release(); o = NULL; } 

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

enum class BlendMode {
	NORMAL,
	ADD_ALPHA,
	REND_TEX
};

class Direct3D {
  public:
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		IDXGISwapChain* getSwapChain();

		void clear();
		void present();
		void setTargetView();
		void setDepthEnable(bool depthEnable);
		void setBlendMode(BlendMode mode);
		void setViewport(float width, float height);

  private:
		ID3D11Device* _device = NULL;
		ID3D11DeviceContext* _deviceContext = NULL;
		IDXGISwapChain* _swapChain = NULL;
		ID3D11Texture2D* _depthStencilTex = NULL;
		ID3D11RenderTargetView* _rtv = NULL;
		ID3D11DepthStencilView* _depthStencilView = NULL;
		ID3D11RasterizerState* _rasterizerState = NULL;
		ID3D11BlendState* _blendStateAlpha = NULL;
		ID3D11BlendState* _blendStateAdd = NULL;
		ID3D11BlendState* _blendStateRendTex = NULL;
		ID3D11DepthStencilState* _depthStencilStateDepthDisable = NULL;
		ID3D11DepthStencilState* _depthStencilStateDepthEnable = NULL;

  public:
		static void CreateInstance(HWND hWnd) {
			DeleteInstance();
			s_instance = new Direct3D(hWnd);
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static Direct3D& GetInstance() {
			return *s_instance;
		}
  private:
		Direct3D(HWND hWnd);
		~Direct3D();
		static inline Direct3D* s_instance;
};

#define DX3D Direct3D::GetInstance()
