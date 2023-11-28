#include "Enemy.h"
#include <cassert>
#include <cmath>
#include <numbers>
#include <algorithm>

Enemy::Enemy(const Vector3& pos)
{
	models_.push_back(std::make_unique<Model>("Resources/EnemyBody", "EnemyBody.obj"));
	models_.push_back(std::make_unique<Model>("Resources/EnemyJoint1", "EnemyJoint1.obj"));

	firstPos_ = pos;
}

void Enemy::Initialize() {

	transform_ = Transform();
	transform_.translate_ = firstPos_;

	models_[kModelIndexBody]->transform_.parent_ = &transform_;
	models_[kModelIndexBody]->transform_.translate_.y = 2.0f;
	models_[kModelIndexUpJoint]->transform_.parent_ = &models_[kModelIndexBody]->transform_;
	models_[kModelIndexUpJoint]->transform_.translate_.y = 3.0f;

	InitializeRotGimmick();

	obb_.size = models_[Joints::kModelIndexBody]->transform_.scale_;

	life_ = 3;
	hitCombo_ = -1;
	parameter_ = 0.0f;

	isDie_ = false;
	isHit_ = false;
	color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	SetColor();
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

	obb_.center = models_[Joints::kModelIndexBody]->transform_.worldPos_;
	obb_.SetOrientations(models_[Joints::kModelIndexBody]->transform_.worldMat_);
}

void Enemy::SetColor()
{
	for (std::unique_ptr<Model>& model : models_) {
		model->color_ = color_;
	}
}

void Enemy::Update() {

	if (isDie_) {
		color_.w = std::clamp<float>(color_.w - 0.03f, 0.0f, 1.0f);
		SetColor();

		transform_.translate_ += skipVector_.Normalize() * 0.5f;
	}
	else if (!isHit_) {
		UpdateRotGimmick();

		const float speed = 0.2f;

		const float pi = 3.14f;

		const float angle = pi / 100.0f;

		transform_.rotate_.y += angle;

		transform_.rotate_.y = std::fmod(transform_.rotate_.y, 2.0f * pi);

		Vector3 move = { 0.0f, 0.0f, -1.0f };

		move *= speed;

		move = move * Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_);

		transform_.translate_ += move;
	}
	else {
		count_++;

		if (count_ == 120) {
			isHit_ = false;
		}
	}

	UpdateMat();
}

void Enemy::Draw(const Matrix4x4& viewProjection) 
{
	if (color_.z != 0) {
		for (std::unique_ptr<Model>& model : models_) {
			model->Draw(viewProjection);
		}
	}
}