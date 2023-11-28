#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <array>

class Particle {
public:

	Particle();

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

	void Begin(const Vector3& pos);

private:

	std::array<std::unique_ptr<Model>,10> models_;
	std::array<Transform, 10> transforms_;
	std::array<Vector3, 10> velcities_;
	int count_;
	bool isDraw_;
};


