#pragma once
#include "CollisionSystem/BaseColliderShapeType/BaseColliderShapeType.h"

class ColliderShapeBox2D : public BaseColliderShapeType
{
public:

	enum class Plane
	{
		XY,
		XZ,
		YZ
	};

	enum v2Info
	{
		CENTER,
		SCALE,
		VELOCITY,
		V2COUNT,
	};

	static bool IsCollisionBox2DBox2D(BaseColliderShapeType* a, BaseColliderShapeType* b);

	static bool IsCollisionBox2DBox2D(const Vector2& pos0, const Vector2& scale0, const Vector2& pos1, const Vector2& scale1);

	ColliderShapeBox2D(Plane plane = Plane::XY);

	void SetV2Info(const Vector3& center, const Vector3& scale, const Vector3& velocity = {});


public:

	Plane plane_;
};

