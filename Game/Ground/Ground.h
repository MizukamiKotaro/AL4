#pragma once
#include "ModelCommon/Model/Model.h"
#include <memory>


class Ground {
public:

	Ground();

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	const std::vector<std::unique_ptr<Model>>& GetModels() { return models_; }

private:
	// モデル
	//std::unique_ptr<Model> model_;

	std::vector<std::unique_ptr<Model>> models_;

	float moveParameter_;
};
