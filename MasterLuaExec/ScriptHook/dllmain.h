#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <iostream>
#include <sstream>

#include "Detours/include/detours.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"

#include "Utility/Renderer.h"
#include "Utility/Input.h"
#include "Utility/Settings.h"
#include "Utility/XorStr.h"

#include "ScriptHook.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#ifndef _WIN64
#pragma comment(lib,"Detours\\x86\\detours.lib")
#else
#pragma comment(lib,"Detours\\x64\\detours.lib")
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

DWORD WINAPI ThreadFunc(LPVOID);