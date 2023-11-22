#include "IScene.h"

// タイトルシーンで初期化
int IScene::sceneNo_ = TITLE;

IScene::~IScene(){}

int IScene::GetSceneNo() { return sceneNo_; }