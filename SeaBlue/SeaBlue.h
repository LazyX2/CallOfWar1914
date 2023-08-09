#pragma once

#include "framework.h"
#include "SeaBlueCore.h"
#include "Graphics.h"

using namespace SeaBlue;
using namespace std;

wstring flag_names[3] = { L"usa", L"uk", L"ottoman" };
wstring testTxt = L"Test";
ID2D1Bitmap* img[3];
ID2D1Bitmap* world_map;
ID2D1BitmapBrush* texture_brush;
int number = 0;

vector<string> split(string str, char c) {
    vector<string> result;
    string token;
    std::istringstream ss(str);
    while (std::getline(ss, token, c)) {
        result.push_back(token);
    }
    return result;
}

vec offset(0, 0);
float scale = 1.0f;
vec clicked;
label* info;
vec worldSize(0, 0);
unordered_map<vec, Core::tile, vec_hash> worldMap = {};
map<string, color> countries_colors = {
    {"unclaimed", color(0.88f, 0.88f, 0.88f)}
    /*
    {"sea", color(128,153,179)},
    {"usa", color(0,162,232)},
    {"canada", color(200,0,0)},
    {"mexico", color(200,50,50)},
    {"uk", color(200,0,0)},
    {"ottoman empire", color(150,0,0)}
    */
};