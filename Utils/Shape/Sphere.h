#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include <cstdint>

struct Sphere
{
public:
	/*Sphere(const Vector3& center, const float& radius);

	Sphere(const Vector3& center, const Vector3& rotate, const float& radius);

	Sphere(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius);*/

	void Initialize(const Vector3& center, const float& radius);
	void Initialize(const Vector3& center, const Vector3& rotate, const float& radius);
	void Initialize(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius);

	void Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

private:
	

public:
	
	Vector3 center_;
	Vector3 scale_;
	Vector3 rotate_;
	float radius_;
};

