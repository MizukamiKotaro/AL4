#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>

class Goal {
public:

	Goal();

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

private:

	Transform transform_;

	std::unique_ptr<Model> model_;

	float parameter_;
};
