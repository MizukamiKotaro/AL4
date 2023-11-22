#pragma once

#include "SceneSystem/IScene/IScene.h"

class ClearScene : public IScene
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};
