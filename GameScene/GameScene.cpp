#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	//dxCommon_ = DirectXCommon::GetInstance();
	camera_.Initialize();
	camera_.transform_.translate_.y = 2.0f;
	
	player_ = std::make_unique<Player>();
	player_->Initialize();
	
	followCamera_.Initialize();
	followCamera_.SetTarget(&player_->GetTransform());

	player_->SetViewProjection(&followCamera_.GetCamera());

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize();
}

void GameScene::Update() {
	ground_->Update();

	goal_->Update();

	enemy_->Update();

	player_->Update();

	const std::vector<std::unique_ptr<Model>>& groundModels = ground_->GetModels();

	for (const std::unique_ptr<Model>& groundModel : groundModels) {
		player_->EditWorldTransform(&groundModel->transform_);
	}

	player_->Collision(&enemy_->GetTransform());
	player_->Collision(&goal_->GetTransform());

	followCamera_.Update();

	const Camera& camera = followCamera_.GetCamera();
	camera_ = followCamera_.GetCamera();
	
}

void GameScene::Draw()
{


	skydome_->Draw(camera_.GetViewProjection());

	ground_->Draw(camera_.GetViewProjection());

	player_->Draw(camera_.GetViewProjection());

	enemy_->Draw(camera_.GetViewProjection());

	goal_->Draw(camera_.GetViewProjection());

}
