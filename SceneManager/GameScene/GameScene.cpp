#include "GameScene.h"
#include <cassert>


GameScene::GameScene() {
	player_ = std::make_unique<Player>();
	enemies_.emplace_back(std::make_unique<Enemy>(Vector3{10.0f,0.0f,120.0f}));
	enemies_.emplace_back(std::make_unique<Enemy>(Vector3{ 20.0f,0.0f,110.0f }));
	enemies_.emplace_back(std::make_unique<Enemy>(Vector3{ -10.0f,0.0f,120.0f }));
	enemies_.emplace_back(std::make_unique<Enemy>(Vector3{ 40.0f,0.0f,110.0f }));
	enemies_.emplace_back(std::make_unique<Enemy>(Vector3{ -30.0f,0.0f,110.0f }));
	ground_ = std::make_unique<Ground>();
	skydome_ = std::make_unique<Skydome>();
	goal_ = std::make_unique<Goal>();
	lockOn_ = std::make_unique<LockOn>();
	particle_ = std::make_unique<Particle>();
}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	camera_.Initialize();
	camera_.transform_.translate_.y = 2.0f;
	
	player_->Initialize();
	particle_->Initialize();
	
	followCamera_.Initialize();
	followCamera_.SetTarget(&player_->GetTransform());

	player_->SetViewProjection(&followCamera_.GetCamera());

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Initialize();
	}

	skydome_->Initialize();

	ground_->Initialize();

	goal_->Initialize();

	lockOn_->Initialize();

	followCamera_.SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());
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
		player_->Collision(enemy.get(), particle_.get());
	}

	particle_->Update();

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

	particle_->Draw(camera_.GetViewProjection());

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw(camera_.GetViewProjection());
	}

	goal_->Draw(camera_.GetViewProjection());

}
