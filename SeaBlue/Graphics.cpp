#include "Graphics.h"

namespace SeaBlue {

	Graphics::Graphics() {
		factory = NULL;
		wicFactory = NULL;
		dwFactory = NULL;
		renderTarget = NULL;
		textFormat = NULL;
	}

	Graphics::~Graphics() {
		SafeRelease(factory);
		SafeRelease(wicFactory);
		SafeRelease(dwFactory);
		SafeRelease(renderTarget);
		SafeRelease(textFormat);
	}

	bool Graphics::Init(HWND hwnd) {
		if (!hwnd) return false;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

		if (hr != S_OK) return false;
		try {
			hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)&wicFactory
			);
		}
		catch (exception e) {
			OutputDebugString((LPCWSTR)e.what());
		}
		if (hr != S_OK) return false;

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(dwFactory),
			reinterpret_cast<IUnknown**>(&dwFactory)
		);

		if (hr != S_OK) return false;

		hr = dwFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			25,
			L"",
			&textFormat
		);
		if (hr != S_OK) return false;

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		if (hr != S_OK) return false;

		RECT rect;
		GetWindowRect(hwnd, &rect);

		hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rect.right, rect.bottom)),
			&renderTarget
		);

		return hr == S_OK;
	}

	void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a) {
		if (!renderTarget) return;
		ID2D1SolidColorBrush* brush = Graphics::CreateBrush(r, g, b, a);
		renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
		brush->Release();
	}

	void Graphics::DrawLine(float fx, float fy, float tx, float ty, float r, float g, float b, float a) {
		if (!renderTarget) return;
		ID2D1SolidColorBrush* brush = Graphics::CreateBrush(r, g, b, a);
		renderTarget->DrawLine(D2D1::Point2F(fx, fy), D2D1::Point2F(tx, ty), brush);
		brush->Release();
	}

	ID2D1SolidColorBrush* Graphics::CreateBrush(float r, float g, float b, float a) {
		if (!renderTarget) return NULL;
		ID2D1SolidColorBrush* brush;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
		return brush;
	}

	ID2D1Bitmap* Graphics::LoadImage(PCWSTR path) {
		if (!wicFactory) return NULL;
		if (!renderTarget) return NULL;
		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICStream* pStream = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		ID2D1Bitmap* bitmap = NULL;

		HRESULT hr = wicFactory->CreateDecoderFromFilename(
			path,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (hr != S_OK) return NULL;

		hr = pDecoder->GetFrame(0, &pSource);

		if (hr != S_OK) return NULL;

		hr = wicFactory->CreateFormatConverter(&pConverter);


		if (hr != S_OK) return NULL;

		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		if (hr != S_OK) return NULL;

		hr = renderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&bitmap
		);

		SafeRelease(pDecoder);
		SafeRelease(pSource);
		SafeRelease(pStream);
		SafeRelease(pConverter);
		SafeRelease(pScaler);
		return bitmap;
	}

	void Graphics::DrawRect(float x, float y, float sx, float sy, float r, float g, float b, float a) {
		if (!renderTarget) return;
		ID2D1SolidColorBrush* brush = CreateBrush(r, g, b, a);
		renderTarget->DrawRectangle(D2D1::RectF(x, y, x + sx, y + sy), brush, 0.0f);
		brush->Release();
	}

	void Graphics::FillRect(float x, float y, float sx, float sy, float r, float g, float b, float a) {
		if (!renderTarget) return;
		ID2D1SolidColorBrush* brush = CreateBrush(r, g, b, a);
		renderTarget->FillRectangle(D2D1::RectF(x, y, x + sx, y + sy), brush);
		renderTarget->DrawRectangle(D2D1::RectF(x, y, x + sx, y + sy), brush);
		brush->Release();
	}

	void Graphics::DrawImage(float x, float y, float sx, float sy, ID2D1Bitmap* img) {
		if (!img) return;
		if (!renderTarget) return;
		renderTarget->DrawBitmap(img, D2D1::RectF(x, y, x + sx, y + sy));
	}
	void Graphics::DrawString(float x, float y, float r, float g, float b, float a, const WCHAR* str) {
		if (!renderTarget) return;
		size_t strLen = wcslen(str);

		renderTarget->DrawTextW(
			str,
			static_cast<UINT32>(strLen),
			textFormat,
			D2D1::RectF(x, y, x + strLen * textFormat->GetFontSize(), y + textFormat->GetFontSize()),
			CreateBrush(r, g, b, a)
		);
	}
}