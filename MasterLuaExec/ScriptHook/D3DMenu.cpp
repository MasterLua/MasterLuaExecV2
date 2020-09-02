#include "D3DMenu.h"
#include "CustomAPI.h"
#include <thread>
#include <iostream>
#include <fstream>  
#include <Windows.h>
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "ImGuiFileDialog/ImGuiFileDialogConfig.h"

using namespace std;

bool bShowWindow = false;
bool bCheckBox1 = false;
bool bCheckBox2 = false;

using RunFileInternal_t = int(*)(uint64_t, const char*, std::function<int(char*)>);
using LoadSystemFileInternal_t = int(*)(uint64_t, const char*);
char LSFIShell[] = { 0x55, 0x56, 0x57, 0x53, 0x48, 0x83, 0xEC, 0x38, 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69 };

uint64_t csLuaBase;
uint64_t grabbedInstance;
uint64_t allocLSFI;


int _fastcall LoadSystemFileInternal(uint64_t luaRuntime, const char* scriptFile) {
	if (!allocLSFI) {
		allocLSFI = reinterpret_cast<uint64_t>(VirtualAlloc(NULL, sizeof(LSFIShell), MEM_COMMIT, 0x40));
		memcpy((void*)allocLSFI, (void*)LSFIShell, sizeof(LSFIShell));
		*(uint64_t*)(allocLSFI + 14) = csLuaBase + 0x289A8;
	}

	return ((LoadSystemFileInternal_t)(allocLSFI))(luaRuntime, scriptFile);
}

int LoadSystemFile(uint64_t luaRuntime, const char* scriptFile) {
	*(BYTE*)(CustomAPI::GetModuleA("adhesive") + 0x49105C) = 1;
	auto result = ((RunFileInternal_t)(csLuaBase + 0x28A90))(luaRuntime, scriptFile, std::bind(&LoadSystemFileInternal, luaRuntime, std::placeholders::_1));
	return result;
}

void StyleDarkAndPurple()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

}



void DrawD3DMenuMain()
{
	StyleDarkAndPurple();
	DrawD3DMenu(10, 20, 848, 435);
	KeyBoard();
}


void DrawD3DMenu(float x, float y, float width, float height)
{
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowBgAlpha(1.0f);
	std::thread([&] {
		csLuaBase = CustomAPI::GetModuleA("citizen-scripting-lua");
			uint64_t* c1 = (uint64_t*)(csLuaBase + 0x63CF80);
			if (*c1 != 0)
				grabbedInstance = *c1;
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}).detach();

	if (bShowWindow)
	{
		ImGui::Begin("MasterLua Exec", NULL, NULL);
		static int tabb = 1;
		{ImGui::SameLine();
		if (ImGui::Button(XorStr("Menu"), ImVec2(150, 25)))
		{
			tabb = 1;
		}
		ImGui::SameLine();

		if (ImGui::Button(XorStr("Editor"), ImVec2(150, 25)))
		{
			tabb = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Dumper"), ImVec2(150, 25)))
		{
			tabb = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Function"), ImVec2(150, 25)))
		{
			tabb = 4;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Misc"), ImVec2(150, 25)))
		{
			tabb = 3;
		}
		}
		if (tabb == 1) {
			if (ImGui::Button("INJECT LUA FILE", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
				igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Menu", ".lua", ".");

			// display
			if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
			{
				// action if OK
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
				{
					std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
					LoadSystemFile(grabbedInstance, filePathName.c_str());
				}
				// close
				igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			}
			if (ImGui::Button("MASTERLUA PUB", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/pub.lua");
			}

			if (ImGui::Button("LOKIMENU", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/lokimenu.lua");
			}
			if (ImGui::Button("LOKI V2", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/lokiv2.lua");
			}
			if (ImGui::Button("RIOT", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/riot.lua");
			}
			if (ImGui::Button("INFINITY", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/infinity.lua");
			}
			if (ImGui::Button("WAVE", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/wave.lua");
			}
			if (ImGui::Button("OBLIVIUSV2", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/obl.lua");
			}
			if (ImGui::Button("DOPAMINE", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/dopa.lua");
			}
			if (ImGui::Button("LUX", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/lux.lua");
			}
			if (ImGui::Button("ATG", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/atg.lua");
			}



		}
		else if (tabb == 2) {
			if (ImGui::Button("PRINT IP IN F8", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:/Users/Public/MasterLuaExec/ip.lua");
			}
			if (ImGui::Button("OPEN DUMPER", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				system("START C:\\Users\\Public\\MasterLuaDumper\\dump.exe");
			}


		}
		else if (tabb == 3) {
			if (ImGui::Button("DISCORD OFFCIAL", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				system("START https://discord.gg/ZcECpcg");
			}
			if (ImGui::Button("DISCORD RESSELER OFFICAL", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				system("START https://discord.gg/F5rXPcN");
			}
			if (ImGui::Button("DEVELOPPER", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				MessageBoxA(NULL, "EXEC DEV By MasterLua\nOfficial Resseler : LokiModz , Peche\nMy best Friends : SlayzTy\nKiss Enjoy", "DEV", NULL);
			}
			ImGui::Checkbox(u8"Anticheat Blocker", &bCheckBox1);
			ImGui::Checkbox(u8"Anti Crash", &bCheckBox2);
		}

		else if (tabb == 0) {
			if (ImGui::Button("OPEN EDITOR", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				system("echo.> C:\\Users\\Public\\custom.lua");
				system("START C:\\Users\\Public\\custom.lua");
			}

			if (ImGui::Button("INJECT", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
			{
				LoadSystemFile(grabbedInstance, "C:\\Users\\Public\\custom.lua");
			}
		}
		else if (tabb == 4) {
		if (ImGui::Button("Kick All", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
		{
			LoadSystemFile(grabbedInstance, "C:\\vo\\kickall.lua");
		}
		if (ImGui::Button("Bypass Password", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
		{
			LoadSystemFile(grabbedInstance, "C:\\vo\\bypasspass.lua");
		}
		if (ImGui::Button("Notification Custom", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
		{
			LoadSystemFile(grabbedInstance, "C:\\vo\\notifannounce.lua");
		}
		if (ImGui::Button("Announcement Custom", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
		{
			LoadSystemFile(grabbedInstance, "C:\\vo\\annoncetabac.lua");
		}
		if (ImGui::Button("TP Players", ImVec2(ImGui::GetWindowSize().x * 1.0f, 0.0f)))
		{
			LoadSystemFile(grabbedInstance, "C:\\vo\\tp.lua");
		}

	}
		ImGui::End();
	}
}


void KeyBoard()
{
	if (GetAsyncKeyState(VK_RMENU) & 1) {
		bShowWindow = !bShowWindow;
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = VK_F8;
		//For key press Flag=0
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));
		//For key relese Flag = KEYEVENTF_KEYUP
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (bShowWindow) {
			io.MouseDrawCursor = true;
		}
		else if (!bShowWindow) {
			keybd_event(VK_F8, 0, 0, 0); // Press F8 Debug for fucking Lokimodz
			io.MouseDrawCursor = false;
		}
	}

}