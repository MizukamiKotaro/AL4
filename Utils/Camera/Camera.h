#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "../Transform/Transform.h"

class Camera {

public:

	Camera();
	~Camera() = default;

	void Initialize();

	void Update();

	const Matrix4x4 GetViewProjection() const { return viewProjectionMatrix_; }

	const Matrix4x4 GetViewMat() const { return viewMatrix_; }

public:

	Transform transform_;

private:

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

};