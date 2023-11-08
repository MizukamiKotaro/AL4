#pragma once
#include "Utils/Camera/Camera.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <string>

class FollowCamera {
public:

	FollowCamera();

	void Initialize();

	void Update();

	const Camera& GetCamera() { return camera_; }

	void SetTarget(const Transform* target);

	void Reset();

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	// 追従対象からのオフセットを計算する
	Vector3 CalcOffset() const;
private:
	const Transform* target_ = nullptr;

	Camera camera_;

	// 追従対象の残像座標
	Vector3 interTarget_;

	// 目標角度
	float destinationAngleY_;

	float interpolationRate_;

	const std::string groupName_ = "FollowCamera";
};
