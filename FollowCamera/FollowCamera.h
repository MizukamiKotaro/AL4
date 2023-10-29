#pragma once
#include "../Camera/Camera.h"
#include "../Transform/Transform.h"
#include <memory>

class FollowCamera {
public:

	FollowCamera();

	void Initialize();

	void Update();

	const Camera& GetCamera() { return camera_; }

	void SetTarget(const Transform* target) { target_ = target; }

private:
	const Transform* target_ = nullptr;

	Camera camera_;
};
