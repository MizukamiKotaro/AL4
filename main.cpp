#include "Kyoko/WinApp/WinApp.h"
#include "Kyoko/ResourceLeakChecker.h"
#include "Kyoko/DirectXCommon.h"
#include "Input/Input.h"
#include "Kyoko/SpriteCommon.h"
#include "Kyoko/Sprite.h"
#include "Kyoko/TextureManager.h"
#include "Kyoko/ImGuiManager.h"
#include "externals/imgui/imgui.h"
#include "Kyoko/ModelCommon.h"
#include "Kyoko/Model.h"
#include "Camera/Camera.h"

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

	std::unique_ptr<Camera> camera = std::make_unique<Camera>();

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	//sprite->Initialize();
	sprite->LoadTexture("Resources/uvChecker.png");

	std::unique_ptr<Sprite> sprite2 = std::make_unique<Sprite>();
	//sprite->Initialize();
	sprite2->LoadTexture("Resources/uvChecker.png");

	std::unique_ptr<Model> model = std::make_unique<Model>("Resources", "fence.obj");

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

#pragma endregion 基盤システムの更新


#pragma region 最初のシーンの更新

		camera->Update();

		ImGui::Begin("sprite");
		ImGui::DragFloat3("pos", &model->pos_.x, 0.01f);
		ImGui::DragFloat3("scale", &model->scale_.x, 0.01f);
		ImGui::DragFloat3("rotate", &model->rotate_.x, 0.01f);
		ImGui::DragFloat3("cameraPos", &camera->pos_.x, 0.01f);
		ImGui::DragFloat3("cameraScale", &camera->scale_.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &camera->rotate_.x, 0.01f);
		ImGui::End();

		sprite2->rotate_.y += 0.01f;

		sprite->Update();
		sprite2->Update();

		model->Update();

#pragma endregion 最初のシーンの更新

		ImGuiManager::End();

		// 描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画

		//sprite->Draw();
		//sprite2->Draw();
		model->Draw(camera->GetViewProjection());


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

