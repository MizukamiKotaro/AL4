#include "ImGuiManager.h"
#include "../externals/imgui/imgui.h"
#include "../externals/imgui/imgui_impl_dx12.h"
#include "../externals/imgui/imgui_impl_win32.h"
#pragma comment(lib, "winmm.lib")
#include "../Kyoko/DirectXCommon/DirectXCommon.h"
#include "../Kyoko/WinApp/WinApp.h"
#include "../TextureManager/TextureManager.h"

//ImGuiManager* ImGuiManager::GetInstance()
//{
//	static ImGuiManager instance;
//	return &instance;
//}

void ImGuiManager::Initialize()
{
	ID3D12DescriptorHeap* srvHeap = TextureManager::GetInstance()->GetSRVHeap();

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WinApp::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(DirectXCommon::GetInstance()->GetDevice(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap,
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::Finalize()
{
	//解放処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	//実際のcommandListのImGuiの描画コマンドを積む。描画処理の終わったタイミング
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetInstance()->GetCommandList());
}

