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

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = Quaternion::Identity();
	Quaternion conj = q1.Conjugate();
	Quaternion inv = q1.Inverse();
	Quaternion normal = q1.Normalize();
	Quaternion mul1 = q1 * q2;
	Quaternion mul2 = q2 * q1;
	float norm = q1.Norm();

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
		ScreenPrint("Identity", identity);
		ScreenPrint("Conjugate", conj);
		ScreenPrint("Inverse", inv);
		ScreenPrint("Normalize", normal);
		ScreenPrint("Multiply(q1,q2)", mul1);
		ScreenPrint("Multiply(q2,q1)", mul2);
		ScreenPrint("Norm", norm);

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

