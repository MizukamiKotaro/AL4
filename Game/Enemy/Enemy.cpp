#include "Enemy.h"
#include <cassert>
#include <cmath>
#include <numbers>

Enemy::Enemy()
{
	models_.push_back(std::make_unique<Model>("Resources/EnemyBody", "EnemyBody.obj"));
	models_.push_back(std::make_unique<Model>("Resources/EnemyJoint1", "EnemyJoint1.obj"));

	parameter_ = 0.0f;
}

void Enemy::Initialize() {

	transform_ = Transform();
	transform_.translate_.z = 130.0f;
	transform_.translate_.x = 10.0f;

	models_[kModelIndexBody]->transform_.parent_ = &transform_;
	models_[kModelIndexBody]->transform_.translate_.y = 2.0f;
	models_[kModelIndexUpJoint]->transform_.parent_ = &models_[kModelIndexBody]->transform_;
	models_[kModelIndexUpJoint]->transform_.translate_.y = 3.0f;

	InitializeRotGimmick();

}

void Enemy::InitializeRotGimmick() { parameter_ = 0.0f; }

void Enemy::UpdateRotGimmick() {

	const uint16_t cycle = 100;

	const float pi = std::numbers::pi_v<float>;

	const float step = 2.0f * pi / cycle;

	parameter_ += step;

	parameter_ = std::fmod(parameter_, 2.0f * pi);
	// 浮遊の振幅
	const float amplitude = 0.1f;

	models_[kModelIndexUpJoint]->transform_.translate_.y += std::sinf(parameter_) * amplitude;

	models_[kModelIndexUpJoint]->transform_.rotate_.y = std::sinf(parameter_);
}

void Enemy::UpdateMat()
{
	transform_.UpdateMatrix();

	for (std::unique_ptr<Model>& model : models_) {
		model->Update();
	}
}

void Enemy::Update() {


	UpdateRotGimmick();

	const float speed = 0.2f;

	const float pi = 3.14f;

	const float angle = pi / 100.0f;

	transform_.rotate_.y += angle;

	transform_.rotate_.y = std::fmod(transform_.rotate_.y, 2.0f * pi);

	Vector3 move = {0.0f, 0.0f, -1.0f};
	
	move *= speed;

	move = move * Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_);

	transform_.translate_ += move;

	UpdateMat();
}

void Enemy::Draw(const Matrix4x4& viewProjection) 
{
	for (std::unique_ptr<Model>& model : models_) {
		model->Draw(viewProjection);
	}
}