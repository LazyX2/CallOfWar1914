#include "SeaBlue.h"

using namespace std;

static const wstring szClassName = L"SeaBlueEngine";

using namespace SeaBlue;
using namespace SeaBlue::Math;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    cout << "Test" << endl;
    if (SUCCEEDED(CoInitialize(NULL))){
        {
            App app;
            if (SUCCEEDED(app.Initialize())) {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}

HRESULT App::Initialize() {
    HRESULT hr;

    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = App::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.lpszClassName = L"SeaBlueEngine";

    RegisterClassEx(&wcex);

    float dpiX, dpiY;
    dpiX = (float)GetDpiForWindow(GetDesktopWindow());
    dpiY = dpiX;

    graphics = new Graphics();

    hWnd = CreateWindowW(
        L"SeaBlueEngine",
        L"Sea Blue",
        WS_OVERLAPPEDWINDOW,
        10,
        10,
        static_cast<UINT>(ceil(640.f * dpiX / 96.0f)),
        static_cast<UINT>(ceil(480.f * dpiY / 96.0f)),
        NULL,
        NULL,
        HINST_THISCOMPONENT,
        this
    );
    hr = hWnd ? S_OK : E_FAIL;
    if (hr != S_OK) return hr;

    if (!graphics->Init(hWnd)) {
        delete graphics;
        return -1;
    }

    CreateDeviceResources();

    ShowWindow(hWnd, SW_SHOWNORMAL);

    UpdateWindow(hWnd);
    return hr;
}

void App::CreateDeviceResources() {
    wstring file_name;
    for (int i = 0; i < 3; i++) {
        file_name = L"flags/";
        file_name.append(flag_names[i]);
        file_name.append(L"_flag.bmp");
        img[i] = graphics->LoadImage(file_name.c_str());
    }

    ifstream game("start.txt");
    if (!game.is_open()) {
        cerr << "Error opening start.txt" << endl;
        return;
    }
    string line, name;
    vector<string> c, color;
    std::getline(game, line);
    while (!game.eof()) {
        int t = 0, y = 0;
        if (line == "colors:") {
            while (std::getline(game, line) && line != "data:") {
                c = split(line, ':');
                if (c.size() == 2) {
                    name = c[0];
                    color = split(c[1], ',');
                    if (color.size() == 3) {
                        countries_colors[name] = SeaBlue::Math::color(stoi(color[0]) / 255.0f, stoi(color[1]) / 255.0f, stoi(color[2]) / 255.0f);
                    }
                }
            }
        } else if (line == "data:") {
            while (std::getline(game, line)) {
                c = split(line, ',');
                for (int x = 0; x < 500; x++) {
                    worldMap[vec(x, y)] = Core::tile(c[x], 1);
                    x++;
                }
                y++;
            }
        }

    }
}

void App::RunMessageLoop() {
    MSG msg;
    int tick = 0;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void SeaBlue::App::OnClick(WPARAM wParam, LPARAM lParam) {
    POINT cursor;
    RECT rect;
    GetWindowRect(hWnd, &rect);
    GetCursorPos(&cursor);
    ScreenToClient(hWnd, &cursor);

    float x = cursor.x / graphics->GetSize().width * 500;
    float y = cursor.y / graphics->GetSize().height * 500;

    float rgb[3] = { 0.0f, 0.5f, 0.0f };
    clicked = vec(x, y);
    string str = worldMap[clicked].owner.data();
    wstring owner = wstring(str.begin(), str.end());

    info = new label(owner, x, y, rgb);
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    App* app;
    RECT rect;
    POINT cursor;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        app = (App*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(app)
        );
    }
    else {
        app = reinterpret_cast<App*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));

        if (app) {
            switch (message) {
            case WM_SIZE:
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                app->graphics->Resize(width, height);
                app->graphics->SetTransform(D2D1::Matrix3x2F::Scale(app->graphics->GetSize().width / 500, app->graphics->GetSize().height / 500));
            }
            return 0;
            case WM_KEYDOWN: {
                switch (wParam)
                {
                case 0x41:
                    offset.x++;
                case 0x44:
                    offset.y--;
                case 0x57:
                    offset.y--;
                case 0x54:
                    offset.y++;
                default:
                    break;
                }
            }
                           return 0;
            case WM_LBUTTONDOWN: {
                app->OnClick(wParam, lParam);
            }
                               return 0;
            case WM_RBUTTONDOWN: {
                app->OnClick(wParam, lParam);
            }
                               return 0;
            case WM_PAINT: {
                app->graphics->BeginDraw();
                app->graphics->ClearScreen(0.3f, 0.3f, 0.9f);
                scale = GET_WHEEL_DELTA_WPARAM(wParam);
                GetWindowRect(hwnd, &rect);
                color c(0.88f, 0.88f, 0.88f);
                for (int y = 0; y < 500; y++)
                {
                    for (int x = 0; x < 500; x++) {
                        if (worldMap[vec(x, y)].type == 1) {
                            c = countries_colors[worldMap[vec(x, y)].owner];
                            app->graphics->FillRect(
                                offset.x + x, offset.y + y,
                                1, 1,
                                c.r, c.g, c.b, 1.0f
                            );
                        }
                    }
                }
                
                //info->render(app->graphics);
                int index = 0;



                app->graphics->EndDraw();
            }
                         return 0;
            case WM_DESTROY: {
                app->graphics->~Graphics();
                PostQuitMessage(0);
            }
                           return 0;
            }
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}