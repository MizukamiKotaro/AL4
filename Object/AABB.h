#pragma once
#include"../Math/Vector3.h"
#include"../Math/Matrix4x4.h"
#include<cstdint>

class AABB
{
public:
	
	void Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

private:


public:
	Vector3 min;
	Vector3 max;
};

