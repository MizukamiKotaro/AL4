#include "StageScene.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "Novice.h"

StageScene::StageScene()
{
	player_ = std::make_unique<Player>();
	enemy_ = std::make_unique<Enemy>();
}

void StageScene::Init()
{
	player_->Init();
	enemy_->Init();
}

void StageScene::Update()
{
	CollisionManager* collisionManager = CollisionManager::GetInstance();
	collisionManager->Clear();

	player_->Update();
	enemy_->Update();

	collisionManager->CheckCollision();

	if (enemy_->GetIsDead()) {
		sceneNo_ = CLEAR;
	}
}

void StageScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x99ff99ff, kFillModeSolid);

	Novice::ScreenPrintf(0, 0, "Press Spase : Bullet");
	Novice::ScreenPrintf(0, 16, "Press WASD : Move");

	player_->Draw();
	enemy_->Draw();
}
