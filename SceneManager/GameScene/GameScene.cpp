#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	camera_.Initialize();
	camera_.transform_.translate_.y = 2.0f;
	
	player_ = std::make_unique<Player>();
	player_->Initialize();
	
	followCamera_.Initialize();
	followCamera_.SetTarget(&player_->GetTransform());

	player_->SetViewProjection(&followCamera_.GetCamera());

	enemies_.emplace_back(std::make_unique<Enemy>());
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Initialize();
	}

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
}

void GameScene::Update() {

	if (player_->GetIsDie()) {
		player_->Initialize();
		for (const std::unique_ptr<Enemy>& enemy : enemies_) {
			enemy->Initialize();
		}
	}

	lockOn_->Update(enemies_, camera_);

	skydome_->Update();

	ground_->Update();

	goal_->Update();

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}

	player_->Update();

	const std::vector<std::unique_ptr<Model>>& groundModels = ground_->GetModels();

	for (const std::unique_ptr<Model>& groundModel : groundModels) {
		player_->EditWorldTransform(&groundModel->transform_);
	}

	player_->Collision(&goal_->GetTransform());

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		player_->Collision(&enemy->GetTransform());
		player_->Collision(enemy->GetOBB(), enemy->GetIsDiePtr());
	}

	followCamera_.Update();

	const Camera& camera = followCamera_.GetCamera();
	camera_ = followCamera_.GetCamera();
	
}

void GameScene::Draw()
{

	skydome_->Draw(camera_.GetViewProjection());

	ground_->Draw(camera_.GetViewProjection());

	player_->Draw(camera_.GetViewProjection());

	lockOn_->Draw(camera_.GetViewProjection());

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw(camera_.GetViewProjection());
	}

	goal_->Draw(camera_.GetViewProjection());

}
