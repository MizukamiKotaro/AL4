#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <list>
#include <numbers>
#include <cmath>
#include "Utils/Camera/Camera.h"

class Enemy;

class LockOn {
public:

	LockOn();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

	// <summary>
	/// 描画
	/// </summary>
	void Draw(const Matrix4x4& viewProjection);

	const Transform& GetTransform() { return transform_; }

private:

	void SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

	bool IsWithinTarget(const Camera& camera);

private:

	Transform transform_;

	std::unique_ptr<Model> model_;

	const Enemy* target_ = nullptr;

	float parameter_;

	float minDistance_ = 80.0f;
	float maxDistance_ = 150.0f;
	float halfAngle_ = std::numbers::pi_v<float> / 4;
	float sinAngle_ = std::sinf(halfAngle_);
};
