#include "ClearScene.h"
#include "InputManager/InputManager.h"
#include "Novice.h"

void ClearScene::Init()
{
}

void ClearScene::Update()
{
	if (InputManager::GetInstance()->PressedKey(DIK_SPACE)) {
		sceneNo_ = TITLE;
	}
}

void ClearScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0xff9999ff, kFillModeSolid);
	Novice::ScreenPrintf(0, 0, "Press Spase");
}
