#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <vector>
#include "Utils/Shape/OBB.h"

class Enemy {
public:

	Enemy(const Vector3& pos);

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

	bool* GetIsDiePtr() { return &isDie_; }

	const bool GetIsDie() const { return isDie_; }

	OBB GetOBB() { return obb_; }

	const Vector3 GetCenterPos() const { return models_[Joints::kModelIndexBody]->transform_.worldPos_; }

private:
	void InitializeRotGimmick();

	void UpdateRotGimmick();

	void UpdateMat();

	void SetColor();

public:
	int life_;

	int hitCombo_;

	bool isHit_;

	int count_;

	Vector3 skipVector_;

private:
	enum Joints { kModelIndexBody, kModelIndexUpJoint };

	Transform transform_;

	std::vector<std::unique_ptr<Model>> models_;

	float parameter_;

	OBB obb_;

	bool isDie_;

	Vector3 firstPos_;

	Vector4 color_;
};
