#include "DirectWrite.h"
#include "DirectX.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

DirectWrite::DirectWrite() {
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_factory);
	DX3D.getSwapChain()->GetBuffer(0, IID_PPV_ARGS(&_backBuffer));
	
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 96, 96);
	_factory->CreateDxgiSurfaceRenderTarget(_backBuffer, &props, &_rt);
	_rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&_writeFactory));
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(&_fontFactory)
	);
	_fontFactory->CreateFontSetBuilder(&_fontBuilder);
	_fontFactory->CreateFontFileReference(L"./assets/font/meiryo.ttc", nullptr, &_fontFile);
	_fontBuilder->AddFontFile(_fontFile);

	IDWriteFontSet* pFontSet;
	_fontBuilder->CreateFontSet(&pFontSet);
	_fontFactory->CreateFontCollectionFromFontSet(pFontSet, &_fontCollection);
	
	_writeFactory->CreateTextFormat(
		L"meiryo",
		_fontCollection,
		fontSetting->fontWeight,
		fontSetting->fontStyle,
		fontSetting->fontStretch,
		fontSetting->fontSize,
		fontSetting->localeName,
		&_textFormat
	);
	_textFormat->SetTextAlignment(fontSetting->textAlignment);
	_rt->CreateSolidColorBrush(fontSetting->Color, &_solidBrush);
}

DirectWrite::~DirectWrite() {
	delete fontSetting;
	SAFE_RELEASE(_fontCollection);
	SAFE_RELEASE(_fontBuilder);
	SAFE_RELEASE(_fontFile);
	SAFE_RELEASE(_fontFactory);
	SAFE_RELEASE(_textFormat);
	SAFE_RELEASE(_solidBrush);
	SAFE_RELEASE(_writeFactory);
	SAFE_RELEASE(_textLayout);
	SAFE_RELEASE(_rt);
	SAFE_RELEASE(_backBuffer);
	SAFE_RELEASE(_factory);
}

void DirectWrite::setFont(FontData* data) {
	_writeFactory->CreateTextFormat(
		FontList[(int)data->font],
		data->fontCollection,
		data->fontWeight,
		data->fontStyle,
		data->fontStretch,
		data->fontSize,
		data->localeName,
		&_textFormat
	);
	_textFormat->SetTextAlignment(data->textAlignment);
	_rt->CreateSolidColorBrush(data->Color, &_solidBrush);
}

void DirectWrite::drawString(std::wstring wstr, Float2 pos) {
	D2D1_SIZE_F TargetSize = _rt->GetSize();
	_writeFactory->CreateTextLayout(wstr.c_str(), wstr.size(), _textFormat, TargetSize.width, TargetSize.height, &_textLayout);

	// 描画位置の確定
	D2D1_POINT_2F pounts;
	pounts.x = pos.x;
	pounts.y = pos.y;

	_rt->BeginDraw();
	_rt->DrawTextLayout(pounts, _textLayout, _solidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	_rt->EndDraw();
}

void DirectWrite::drawString(std::wstring wstr, D2D1_RECT_F rect) {
	_rt->BeginDraw();
	_rt->DrawText(wstr.c_str(), wstr.size(), _textFormat, rect, _solidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	_rt->EndDraw();
}