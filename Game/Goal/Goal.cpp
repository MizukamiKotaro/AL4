#include "Goal.h"
#include <cassert>
#include <cmath>
#include <numbers>

Goal::Goal()
{
	model_ = std::make_unique<Model>("Resources/Cube", "Cube.obj");

	parameter_ = 0.0f;
}

void Goal::Initialize() {

	transform_ = Transform();
	transform_.translate_.z = 130.0f;
	transform_.translate_.x = 20.0f;

	model_->transform_.parent_ = &transform_;
	model_->transform_.translate_.y = 2.0f;
	
}

void Goal::Update() {

	transform_.UpdateMatrix();
	model_->Update();
	
}

void Goal::Draw(const Matrix4x4& viewProjection) 
{
	model_->Draw(viewProjection);
}