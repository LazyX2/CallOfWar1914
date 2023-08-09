#pragma once

#include "framework.h"
#include "Graphics.h"

using namespace std;
using namespace SeaBlue::Math;

namespace SeaBlue {

    #ifndef HINST_THISCOMPONENT
        EXTERN_C IMAGE_DOS_HEADER __ImageBase;
    #define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
    #endif

    class App {
    public:
        App() :
            hWnd(NULL), graphics() {}
        ~App() {
            graphics->~Graphics();
        }

        HRESULT Initialize();

        void RunMessageLoop();
        
        void OnClick(WPARAM wParam, LPARAM lParam);

    private:
        void CreateDeviceResources();

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        HWND hWnd;
        Graphics* graphics;
    };
}
