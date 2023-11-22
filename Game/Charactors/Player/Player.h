#pragma once
#include "Game/Charactors/BaseCharactor/BaseCharactor.h"
#include "CollisionSystem/ColliderShapes/ColliderShapeBox2D.h"
#include <list>
#include <memory>

class Player : public BaseCharactor
{
public:

	Player();

	void Init() override;

	void Update() override;

	void Draw() override;

	void OnCollision();

private:

	
};

