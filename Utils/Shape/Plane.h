#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include <cstdint>

class Plane {
public:
	void Initialize();

	void Update();

	//void Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

public:
	Vector3 normal; // 法線
	float distance; // 距離
	bool isLimit = false;
};
