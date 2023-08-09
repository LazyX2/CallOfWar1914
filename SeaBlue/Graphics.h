#pragma once

#include "framework.h"

namespace SeaBlue {

    class Graphics
    {
    public:
        Graphics();
        ~Graphics();

        bool Init(HWND hwnd);

        void BeginDraw() { renderTarget->BeginDraw(); }
        void EndDraw() { renderTarget->EndDraw(); }
        void Resize(D2D1_SIZE_U size) { renderTarget->Resize(size); }
        void Resize(UINT width, UINT height) { renderTarget->Resize(D2D1::SizeU(width, height)); }
        void SetTransform(D2D_MATRIX_3X2_F matrix) { renderTarget->SetTransform(matrix); }

        void ClearScreen(float r, float g, float b) { renderTarget->Clear(D2D1::ColorF(r, g, b)); }
        void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
        void DrawLine(float fx, float fy, float tx, float ty, float r, float g, float b, float a);
        void DrawRect(float x, float y, float sx, float sy, float r, float g, float b, float a);
        void FillRect(float x, float y, float sx, float sy, float r, float g, float b, float a);
        void DrawImage(float x, float y, float sx, float sy, ID2D1Bitmap* img);
        void DrawString(float x, float y, float r, float g, float b, float a, const WCHAR* str);

        ID2D1SolidColorBrush* CreateBrush(float r, float g, float b, float a);
        ID2D1Bitmap* LoadImage(PCWSTR path);
        D2D1_WINDOW_STATE CheckWindowState() { return renderTarget->CheckWindowState(); }
        D2D1_SIZE_F GetSize() { return renderTarget->GetSize(); }

    private:
        ID2D1Factory* factory;
        IWICImagingFactory* wicFactory;
        IDWriteFactory* dwFactory;
        ID2D1HwndRenderTarget* renderTarget;
        IDWriteTextFormat* textFormat;
    };
    struct drawable {
        void render(Graphics* g);
    };

    struct container : drawable {
        drawable* drawables;
        void render(Graphics* g) {
            for (int i = 0; i < sizeof(drawables); i++) {
                drawables[i].render(g);
            }
        }
    };

    struct label : drawable {
        wstring text;
        float x, y;
        float rgb[3];
        label(wstring text, float x, float y, float bg[3]) {
            this->x = x;
            this->y = y;
            this->text = text;
            for (int i = 0; i < 3; i++)
                this->rgb[i] = bg[i];
        }

        void render(Graphics* g) {
            if (!this) return;
            g->DrawString(
                x, y,
                rgb[0], rgb[1], rgb[2], 1.0f, text.data()
            );
        }
    };

    struct button : label {
        virtual void click(float x, float y);
    };
}