#include "SceneManager.h"
#include "Novice.h"
#include "InputManager/InputManager.h"

SceneManager::SceneManager()
{
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	IScene::sceneNo_ = TITLE;
	currentSceneNo_ = TITLE;
}

SceneManager::~SceneManager()
{
}

int SceneManager::Run()
{
	
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Init();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		inputManager->Update();

		///
		/// ↓更新処理ここから
		///
		/// 
		preSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		if (preSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Init();
		}

		sceneArr_[currentSceneNo_]->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		sceneArr_[currentSceneNo_]->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (inputManager->PressedKey(DIK_ESCAPE)) {
			break;
		}
	}

	return 0;
}
