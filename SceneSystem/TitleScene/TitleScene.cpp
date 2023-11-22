#include "TitleScene.h"
#include "InputManager/InputManager.h"
#include "Novice.h"

void TitleScene::Init()
{

}

void TitleScene::Update()
{
	if (InputManager::GetInstance()->PressedKey(DIK_SPACE)) {
		sceneNo_ = STAGE;
	}
}

void TitleScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x9999ffff, kFillModeSolid);
	Novice::ScreenPrintf(0, 0, "Press Spase");
}
