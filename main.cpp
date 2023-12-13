#include "Kyoko/WinApp/WinApp.h"
#include "Kyoko/ResourceLeakChecker.h"
#include "Kyoko/DirectXCommon/DirectXCommon.h"
#include "Input/Input.h"
#include "SpriteCommon/SpriteCommon.h"
#include "SpriteCommon/Sprite/Sprite.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "externals/imgui/imgui.h"
#include "ModelCommon/ModelCommon.h"
#include "SceneManager/GameScene/GameScene.h"
#include "GlobalVariables/GlobalVariables.h"

#include "Utils/ScreenPrint/ScreenPrint.h"
#include "Utils/Math/Quaternion.h"

static ResourceLeackChecker leakCheck;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	CoInitializeEx(0, COINIT_MULTITHREADED);

#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効にする
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // DEBUG

#pragma region 基盤システムの初期化

	WinApp* winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->Initialize(dxCommon->GetDevice());

	ModelCommon* modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize();

	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->LoadFiles();

#pragma endregion 基盤システムの初期化

	ID3D12Device* device_ = dxCommon->GetDevice();

	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラーのときに止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーバグ
			//https:://stackoverflow.com/question/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}
#endif // _DEBUG

#pragma region 最初のシーンの初期化

	
	/*std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize();*/

	Quaternion rot0 = Quaternion::MakeRotateAxisAngle({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion rot1 = -1 * rot0;

	Quaternion interpolate0 = Quaternion::Slerp(rot0, rot1, 0.0f);
	Quaternion interpolate1 = Quaternion::Slerp(rot0, rot1, 0.3f);
	Quaternion interpolate2 = Quaternion::Slerp(rot0, rot1, 0.5f);
	Quaternion interpolate3 = Quaternion::Slerp(rot0, rot1, 0.7f);
	Quaternion interpolate4 = Quaternion::Slerp(rot0, rot1, 1.0f);

#pragma endregion 最初のシーンの初期化
	
	ImGuiManager::Initialize();
	
	//ウィンドウの×ボタンが押されるまでループ
	while (true) {

#pragma region 基盤システムの更新
		//windowにメッセージが来たら最優先で処理させる
		if (winApp->ProcessMessage()) {
			break;
		}
		ImGuiManager::Begin();
		input->Update();

		globalVariables->Update();

#pragma endregion 基盤システムの更新


#pragma region 最初のシーンの更新

		//gameScene->Update();
		ScreenPrint("interpolate0, 0.0f", interpolate0);
		ScreenPrint("interpolate1, 0.3f", interpolate1);
		ScreenPrint("interpolate2, 0.5f", interpolate2);
		ScreenPrint("interpolate3, 0.7f", interpolate3);
		ScreenPrint("interpolate4, 1.0f", interpolate4);

#pragma endregion 最初のシーンの更新

		ImGuiManager::End();

		// 描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画

		
		//gameScene->Draw();


#pragma endregion 最初のシーンの描画
		ImGuiManager::Draw();

		// 描画後処理
		dxCommon->PostDraw();

	}

#pragma region 最初のシーンの終了



#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了
	ImGuiManager::Finalize();

	CoUninitialize();
	textureManager->Finalize();
	dxCommon->Finalize();
	winApp->Finalize();

#ifdef _DEBUG
	debugController->Release();
#endif // _DEBUG

#pragma endregion 基盤システムの終了

	return 0;
}

