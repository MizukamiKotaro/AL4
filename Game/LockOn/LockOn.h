#pragma once
#include "Utils/Transform/Transform.h"
#include <memory>
#include <list>
#include <numbers>
#include <cmath>
#include "Utils/Camera/Camera.h"
#include "SpriteCommon/Sprite/Sprite.h"

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

	const Vector3 GetTargetPos() const;

	const bool ExistTarget() const { return target_ ? true : false; }

private:

	void SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

	bool IsWithinTarget(const Camera& camera);

	void ChangeSearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera);

private:

	Transform transform_;

	bool isAuto_;

	std::unique_ptr<Sprite> sprite_;

	const Enemy* target_ = nullptr;

	float parameter_;

	float minDistance_ = 40.0f;
	float maxDistance_ = 140.0f;
	float halfAngle_ = std::numbers::pi_v<float> / 12;
	float sinAngle_ = std::sinf(halfAngle_);
};
