#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include <cstdint>

class OBB
{
public:

	void Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	void SetOrientations(const Matrix4x4& rotateMat);

public:
	
	Vector3 center;
	Vector3 orientations[3]; // 座標軸。正規化、直交必須
	Vector3 size;
};

