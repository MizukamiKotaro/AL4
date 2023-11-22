#pragma once

#include "SceneSystem/IScene/IScene.h"

class TitleScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};

