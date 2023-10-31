#include "Player.h"
#include <cassert>
#include "../Ease/Ease.h"
#include <cmath>
#include "../Camera/Camera.h"
#include "../Input/Input.h"
#include <numbers>

Player::Player()
{
	models_.push_back(std::make_unique<Model>("Resources/float_Body", "float_Body.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_Head", "float_Head.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_L_Arm", "float_L_Arm.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_R_Arm", "float_R_Arm.obj"));

	camera_ = nullptr;

	isNowOnFloor_ = false;
	isOnFloor_ = false;
	isJump_ = false;
	velocity_ = {};
	floatingParameter_ = 0.0f;
}

void Player::Initialize() 
{
	
	transform_ = Transform();

	models_[kModelIndexBody]->transform_.parent_ = &transform_;
	models_[kModelIndexHead]->transform_.parent_ = &models_[kModelIndexBody]->transform_;
	models_[kModelIndexHead]->transform_.translate_.y = 3.0f;
	models_[kModelIndexL_arm]->transform_.parent_ = &models_[kModelIndexBody]->transform_;
	models_[kModelIndexL_arm]->transform_.translate_ = { 1.0f, 2.5f, 0.0f };
	models_[kModelIndexR_arm]->transform_.parent_ = &models_[kModelIndexBody]->transform_;
	models_[kModelIndexR_arm]->transform_.translate_ = { -1.0f, 2.5f, 0.0f };

	behaviorRequest_ = Behavior::kRoot;

	velocity_ = {};

	floatingParameter_ = 0.0f;

	InitializeFloatingGimmick();

	isJump_ = false;

	isOnFloor_ = true;

	isNowOnFloor_ = true;
}

void Player::UpdateMat()
{
	transform_.UpdateMatrix();

	for (std::unique_ptr<Model>& model : models_) {
		model->Update();
	}
}

void Player::SetWorldTranslateParent(const Transform* transform)
{
	if (!transform_.worldTranslateParent_) {
		transform_.worldTranslateParent_ = transform;
		transform_.translate_ -= transform->worldPos_;
	}

}

void Player::ClearWorldTranslateParent()
{
	if (transform_.worldTranslateParent_) {
		transform_.translate_ += transform_.worldTranslateParent_->worldPos_;
		transform_.worldTranslateParent_ = nullptr;
	}
}

void Player::EditWorldTransform(const Transform* transform)
{

	if (transform_.IsCollisionXZ(*transform)) {
		if (transform_.worldPos_.y <= transform->worldPos_.y && transform_.worldPos_.y - velocity_.y >= transform->worldPos_.y && velocity_.y != 0) {

			float t = transform_.worldPos_.y - transform->worldPos_.y / velocity_.y;

			transform_.translate_ -= velocity_ * (t);
			transform_.translate_.y = transform->worldPos_.y;

			SetWorldTranslateParent(transform);
		
			UpdateMat();
			velocity_ = {};
			isJump_ = false;
			isOnFloor_ = true;
			isNowOnFloor_ = true;
		}
		else {

			if (velocity_.y != 0) {
				ClearWorldTranslateParent();
				UpdateMat();
				isJump_ = true;
				isNowOnFloor_ = false;
			}
			else {

				SetWorldTranslateParent(transform);

				UpdateMat();
				velocity_ = {};
				isJump_ = false;
				isOnFloor_ = true;
				isNowOnFloor_ = true;
				
			}
		}
	}
	else if (!isOnFloor_ && !isNowOnFloor_) {
		ClearWorldTranslateParent();
		UpdateMat();
		isJump_ = true;
	}
	else {
		if (transform_.worldTranslateParent_) {
			if (!transform_.IsCollisionXZ(*transform_.worldTranslateParent_)) {
				ClearWorldTranslateParent();
				UpdateMat();
				isJump_ = true;
				isNowOnFloor_ = false;
			}
		}
	}
}

void Player::Collision(const Transform* transform)
{
	if (transform_.IsCollisionXZ(*transform)) {
		Initialize();
		UpdateMat();
	}

}



void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {

	const uint16_t cycle = 100;

	const float pi = std::numbers::pi_v<float>;

	const float step = 2.0f * pi / cycle;

	floatingParameter_ += step;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	//浮遊の振幅
	const float amplitude = 0.5f;

	models_[kModelIndexBody]->transform_.translate_.y = std::sinf(floatingParameter_) * amplitude;

	const float angle = pi / 6;

	models_[kModelIndexL_arm]->transform_.rotate_.z = std::sinf(floatingParameter_) * angle;
	models_[kModelIndexR_arm]->transform_.rotate_.z = std::sinf(floatingParameter_) * angle;
}

void Player::BehaviorRootInitialize() { InitializeFloatingGimmick(); }

void Player::BehaviorRootUpdate() {
	
	UpdateFloatingGimmick();

	const float speed = 1.0f;

	Vector2 inputVector = Input::GetInstance()->GetGamePadLStick();

	Vector3 move = { inputVector.x, 0.0f,inputVector.y };

	move = move.Normalize() * speed;

	move = move * Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_);

	if (Input::GetInstance()->GetGamePadButtonA() && !isJump_) {
		isJump_ = true;
		velocity_.y += 2.0f;
	}

	velocity_.x = move.x;
	velocity_.z = move.z;

	if (isJump_) {
		velocity_.y -= 0.2f;
	}

	transform_.translate_ += velocity_;

	const float pi = std::numbers::pi_v<float>;

	if (velocity_.x == 0 && velocity_.z == 0) {
		transform_.rotate_.y = camera_->transform_.rotate_.y + pi;
	}
	else {
		transform_.rotate_.y = atan2(velocity_.x, velocity_.z);

		/*Vector3 rotate = Vector3{ velocity_.x,0.0f,velocity_.z }.Normalize();
		Vector3 pos = Vector3{ camera_->transform_.translate_.x,0.0f,camera_->transform_.translate_.z };
		Vector3 pos1 = Vector3{ transform_.translate_.x,0.0f,transform_.translate_.z };
		Vector3 rotate1 = Vector3{ pos - pos1 }.Normalize();*/
		//Matrix4x4 a = Matrix4x4::DirectionToDirection(rotate1, rotate);
		//transform_.rotate_ = (rotate1 *a).Normalize();

		//transform_.SetOtherRotateMatrix(rotate1, rotate);

		// 求めたaの値がtransform_.rotate_.yの回転行列な気がする。matrixにかければ変わるだろうけどUpdateせれて戻るからどうすればいいかわからん。
		// transform_.rotate_の変化前と変化後でmatrix作れればうまくいく気がするけど、変化後のyが不明。

	}

}

void Player::Update() {

	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;

	}

	switch (behavior_) {
	case Behavior::kRoot:
		BehaviorRootUpdate();

		break;
	}

	if (transform_.worldPos_.y <= -20.0f) {
		Initialize();
	}

	UpdateMat();

	isOnFloor_ = false;
}

void Player::Draw(const Matrix4x4& viewProjection) {
	
	for (std::unique_ptr<Model>& model : models_) {
		model->Draw(viewProjection);
	}

}