#pragma once
#include "CollisionSystem/Collider/Collider.h"
#include "Utils/Math/Vector2.h"

class BaseCharactor : public Collider
{
public:

	virtual void Init();

	virtual void Update();

	virtual void Draw();

	virtual void OnCollision() override;

protected:

	Vector2 pos_;
	float radius_;
	Vector2 velocity_;
	unsigned int color_;

};

