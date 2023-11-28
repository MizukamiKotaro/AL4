#include "Ground.h"
#include <cmath>

Ground::Ground()
{
	//model_ = std::make_unique<Model>("Resources/ground", "ground.obj");

	for (int i = 0; i < 3; i++) {
		models_.push_back(std::make_unique<Model>("Resources/ground", "floor.obj"));

		moveParameter_ = 0.0f;
	}
}

void Ground::Initialize()
{
	int i = 0;

	for (std::unique_ptr<Model>& model : models_) {
		model->transform_.scale_ = { 30.0f,1.0f,30.0f };
		model->transform_.translate_.z = i * 65.0f;
		i++;
	}

	models_[0]->transform_.scale_.x = 100.0f;
	models_[2]->transform_.scale_.x = 100.0f;
}

void Ground::Update() {

	const uint16_t cycle = 1000;

	const float pi = 3.14f;

	const float step = 2.0f * pi / cycle;


	moveParameter_ += step;

	moveParameter_ = std::fmod(moveParameter_, 2.0f * pi);
	
	const float amplitude = 80.0f;

	models_[1]->transform_.translate_.x = std::sinf(moveParameter_) * amplitude;

	//model_->Update();

	for (std::unique_ptr<Model>& model : models_) {
		model->Update();
	}
}

void Ground::Draw(const Matrix4x4& viewProjection) {
	//model_->Draw(viewProjection);

	for (std::unique_ptr<Model>& model : models_) {
		model->Draw(viewProjection);
	}
}

