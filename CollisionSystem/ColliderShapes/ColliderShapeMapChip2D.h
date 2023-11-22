#pragma once
#include "Utils/Math/Vector3.h"
#include "CollisionSystem/BaseColliderShapeType/BaseColliderShapeType.h"
#include <vector>

class ColliderShapeMapChip2D : public BaseColliderShapeType
{
public:

	enum class Plane
	{
		XY,
		XZ,
		YZ
	};

	enum iInfo {
		MAXHEIGHTCHIPNUM,
		ICOUNT,
	};

	enum v2Info
	{
		BOTTOMLEFTPOS,
		CHIPSIZE,
		V2COUNT,
	};

	ColliderShapeMapChip2D(std::vector<std::vector<int>> mapChip, int kMaxHeightChipNum, const Vector3& bottomLeftPos, const Vector3& chipSize, Plane plane = Plane::XY);

	void SetMapChip(const std::vector<std::vector<int>>& mapchip);
	void SetNoRigitBody(int no);
	void SetNoCollider(int no);
	bool IsRigitBody(int y, int x);
	bool IsCollider(int y, int x);

public:

	Plane plane_;
};
