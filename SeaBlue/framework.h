#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>

#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <tchar.h>

#include <iostream>
#include <fstream>

#include <string.h>
#include <sstream>
#include <map>
#include <unordered_map>
#include <cmath>
#include <functional>

#include <codecvt>
#include <locale>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "BMPReader.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

#define SafeRelease(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

using namespace std;

namespace SeaBlue {

    namespace Math {

        struct color
        {
            float r,g,b;
            color() {
                r = 0.0f;
                g = 0.0f;
                b = 0.0f;
            }
            color(float x, float y, float z) {
                r = x;
                g = y;
                b = z;
            }
        };

        struct vec {
            int x, y;
            vec() {
                x = 0;
                y = 0;
            }
            vec(int x, int y) {
                this->x = x;
                this->y = y;
            }

            bool operator<(const vec& key) const {
                std::size_t seed = 0;
                seed ^= std::hash<int>()(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= std::hash<int>()(y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

                std::size_t seed1 = 0;
                seed1 ^= std::hash<int>()(key.x) + 0x9e3779b9 + (seed1 << 6) + (seed1 >> 2);
                seed1 ^= std::hash<int>()(key.y) + 0x9e3779b9 + (seed1 << 6) + (seed1 >> 2);
                return seed < seed1;
            }

            bool operator==(const vec& key) const {
                std::size_t seed = 0;
                seed ^= std::hash<int>()(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= std::hash<int>()(y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

                std::size_t seed1 = 0;
                seed1 ^= std::hash<int>()(key.x) + 0x9e3779b9 + (seed1 << 6) + (seed1 >> 2);
                seed1 ^= std::hash<int>()(key.y) + 0x9e3779b9 + (seed1 << 6) + (seed1 >> 2);
                return seed == seed1;
            }
        };

    }

    namespace Core {

        struct tile
        {
            int type;
            string owner, occupier;
            tile() {
                owner = "none";
                occupier = "none";
                this->type = 0;
            }
            tile(string ruler, int type) {
                owner = ruler;
                occupier = ruler;
                this->type = type;
            }
        };

    }

    struct vec_hash {
        size_t operator()(const Math::vec& key) const {
            std::size_t seed = 0;
            seed ^= std::hash<int>()(key.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(key.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}