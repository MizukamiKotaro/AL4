#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"

class Camera {

public:

	Camera();
	~Camera() = default;

	void Update();

	const Matrix4x4 GetViewProjection() { return viewProjectionMatrix_; }

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 pos_;

private:

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

};