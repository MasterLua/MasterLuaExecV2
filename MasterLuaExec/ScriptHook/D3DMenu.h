#pragma once
#include "dllmain.h"
#include "ImGui/imgui.h"
#include <windows.h>
#include <iostream>

void DrawD3DMenuMain();
void DrawD3DMenu(float x, float y, float width, float height);

#ifndef IMGUI_THEME_COLORS_H
#define IMGUI_THEME_COLORS_H

void KeyBoard();

void StyleColorsPurple();
void StyleColorVulkan();
void StyleDarkAndPurple();

#endif //IMGUI_THEME_COLORS_H
