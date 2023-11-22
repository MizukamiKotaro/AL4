#pragma once
#include "Game/Charactors/BaseCharactor/BaseCharactor.h"
#include "CollisionSystem/ColliderShapes/ColliderShapeBox2D.h"

class Enemy : public BaseCharactor
{
public:

	Enemy();

	void Init() override;

	void Update() override;

	void OnCollision();

	bool GetIsDead() const;

private:

	bool isDead_;
};

