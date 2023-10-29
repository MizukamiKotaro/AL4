#pragma once
#include "../Kyoko/Model.h"
#include <memory>

class Skydome {
public: 

	Skydome();

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

private:
	// モデル
	std::unique_ptr<Model> model_;
};
