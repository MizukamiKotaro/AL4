#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <vector>
#include "Utils/Shape/OBB.h"

class Enemy {
public:

	Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// <summary>
	/// 描画
	/// </summary>
	void Draw(const Matrix4x4& viewProjection);

	const Transform& GetTransform() { return transform_; }

	bool* GetIsDie() { return &isDie_; }

	OBB GetOBB() { return obb_; }

private:
	void InitializeRotGimmick();

	void UpdateRotGimmick();

	void UpdateMat();

private:
	enum Joints { kModelIndexBody, kModelIndexUpJoint };

	Transform transform_;

	std::vector<std::unique_ptr<Model>> models_;

	float parameter_;

	OBB obb_;

	bool isDie_;
};
