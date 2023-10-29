#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include<cstdint>

class Triangle
{
public:
	//void Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
public:
	Vector3 vertices[3];
};

