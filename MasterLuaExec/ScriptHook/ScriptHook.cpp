#include "ScriptHook.h"

ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
IDXGISwapChain* pSwapChain;
ID3D11RenderTargetView* pRenderTargetView;

ScriptHook::ScriptHook()
{

}


ScriptHook::~ScriptHook()
{

}

BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lParam)
{
	ProcessWindowData& wndData = *(ProcessWindowData*)lParam;
	unsigned long lProcessId = 0;
	::GetWindowThreadProcessId(hWnd, &lProcessId);
	if ((wndData.lProcessId != lProcessId) || (::GetWindow(hWnd, GW_OWNER) != (HWND)0) || !::IsWindowVisible(hWnd))
		return 1;
	wndData.hWnd = hWnd;
	return 0;
}

HWND GetMainWindowHwnd(unsigned long lProcessId)
{
	ProcessWindowData wndData;
	wndData.hWnd = 0;
	wndData.lProcessId = lProcessId;
	::EnumWindows(EnumWindowCallback, (LPARAM)&wndData);
	return wndData.hWnd;
}

void ScriptHook::Initialize()
{
	HWND hWnd = GetMainWindowHwnd(GetCurrentProcessId());
	RECT rect;
	GetClientRect(hWnd, &rect);

	// 交换链
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;														//	创建双缓冲
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							//	设置颜色格式，使用8位RGBA
	scd.BufferDesc.Width = rect.right - rect.left;
	scd.BufferDesc.Height = rect.bottom - rect.top;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;						//	缩放比
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		//	扫描线
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							//	渲染目标输出
	scd.OutputWindow = hWnd;													//	指定输出窗口
	scd.SampleDesc.Count = 1;													//	重采样
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;							//	允许模式切换
	scd.BufferDesc.RefreshRate.Numerator = 144;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.Windowed = !(GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP);				//	是否全屏
	scd.SampleDesc.Quality = 0;													//	采样等级
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;									//	采样参数

	D3D_FEATURE_LEVEL featrueLevel = D3D_FEATURE_LEVEL_11_1;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featrueLevel, 1, D3D11_SDK_VERSION, &scd, &pSwapChain, &pDevice, NULL, &pContext);

	DWORD_PTR* pSwapChainVT = NULL;
	DWORD_PTR* pDeviceVT = NULL;
	DWORD_PTR* pContextVT = NULL;
	pSwapChainVT = (DWORD_PTR *)pSwapChain;
	// pDeviceVT = (DWORD_PTR*)pDevice;
	// pContextVT = (DWORD_PTR*)pContext;

	pSwapChainVT = (DWORD_PTR *)pSwapChainVT[0];
	Hooks::oPresent = (tD3D11Present)pSwapChainVT[8];

	Renderer::GetInstance()->Initialize();
	Renderer::GetInstance()->SetStyle();
	Input::GetInstance()->StartThread();

	HookFunction((PVOID *)&Hooks::oPresent, (PVOID)&Hooks::hkD3D11Present);
}

void ScriptHook::Release()
{
	Input::GetInstance()->StopThread();
	UnHookFunction((PVOID *)&Hooks::oPresent, (PVOID)&Hooks::hkD3D11Present);
}

void ScriptHook::HookFunction(PVOID * oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}

void ScriptHook::UnHookFunction(PVOID * oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}

namespace Hooks
{
	tD3D11Present oPresent = NULL;
	bool bOnce = false;

	HRESULT __stdcall hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SysInterval, UINT Flags)
	{
		if (!bOnce)
		{
			//第一次会进来
			//得到游戏窗体
			HWND hWindow = GetMainWindowHwnd(GetCurrentProcessId());
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)(&pDevice))))
			{
				pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)(&pDevice));
				pDevice->GetImmediateContext(&pContext);
			}

			ID3D11Texture2D* renderTargetTexture = nullptr;
			//获取后缓冲区地址
			if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&renderTargetTexture)))
			{
				//创建目标视图
				pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &pRenderTargetView);
				//释放后缓冲
				renderTargetTexture->Release();
			}

			//初始化ImGUI
			ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
			ImGui_ImplDX11_CreateDeviceObjects();

			ImGui::StyleColorsDark();

			bOnce = true;
		}

		//不停的画插件界面
		ImGui_ImplDX11_NewFrame();

		DrawD3DMenuMain();

		ImGui::Render();
		pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return oPresent(pSwapChain, SysInterval, Flags);
	}
}