#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Charactors/Player/Player.h"
#include "Charactors/Enemy/Enemy.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;

};
