#include "Skydome.h"

Skydome::Skydome()
{
	model_ = std::make_unique<Model>("Resources/skydome", "skydome.obj");
}

void Skydome::Initialize()
{
}

void Skydome::Update() {
	model_->Update();
}

void Skydome::Draw(const Matrix4x4& viewProjection) {
	model_->Draw(viewProjection);
}
