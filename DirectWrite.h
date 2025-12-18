#pragma once

#include "MathStruct.h"
#include "d2d1.h"
#include <dwrite.h>
#include <dwrite_3.h>
#include <string>

enum class Font {
	Meiryo,
	Arial,
	MeiryoUI
};

namespace {
	const WCHAR* FontList[] {
		L"メイリオ",
		L"Arial",
		L"Meiryo UI"
	};
}

struct FontData {
	Font font;					// フォント名
	IDWriteFontCollection* fontCollection;		// フォントコレクション
	DWRITE_FONT_WEIGHT fontWeight;			// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;			// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;		// フォントの幅
	FLOAT fontSize;					// フォントサイズ
	WCHAR const* localeName;			// ロケール名
	DWRITE_TEXT_ALIGNMENT textAlignment;		// テキストの配置
	D2D1_COLOR_F Color;				// フォントの色

	// デフォルト設定
	FontData() {
		font = Font::Meiryo;
		fontCollection = nullptr;
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		fontSize = 30;
		localeName = L"ja-jp";
		textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		Color = D2D1::ColorF(D2D1::ColorF::Black);
	}
};

class DirectWrite {
	public:
		void setFont(FontData* data);
		void drawString(std::wstring wstr, Float2 pos);
		void drawString(std::wstring wstr, D2D1_RECT_F rect);

	private:
		ID2D1Factory* _factory = NULL;
		IDWriteFactory* _writeFactory = NULL;
		IDWriteTextFormat* _textFormat = NULL;
		IDWriteTextLayout* _textLayout = NULL;
		ID2D1RenderTarget* _rt = NULL;
		ID2D1SolidColorBrush* _solidBrush = NULL;
		IDXGISurface* _backBuffer = NULL;

		IDWriteFactory5* _fontFactory = NULL;
		IDWriteFontSetBuilder1* _fontBuilder = NULL;
		IDWriteFontFile* _fontFile = NULL;
		IDWriteFontCollection1* _fontCollection = NULL;

		FontData* fontSetting = new FontData();

		std::wstring stringToWString(std::string oString);

	public:
		static void CreateInstance() {
			DeleteInstance();
			s_instance = new DirectWrite();
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static DirectWrite& GetInstance() {
			return *s_instance;
		}
	private:
		DirectWrite();
		~DirectWrite();
		static inline DirectWrite* s_instance;
};

#define DWRITE DirectWrite::GetInstance()