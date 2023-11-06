#include "Player.h"
#include <cassert>
#include "../../Utils/Ease/Ease.h"
#include <cmath>
#include "../../Utils/Camera/Camera.h"
#include "../../Input/Input.h"
#include <numbers>
#include "../../GlobalVariables/GlobalVariables.h"

Player::Player()
{
	models_.push_back(std::make_unique<Model>("Resources/float_Body", "float_Body.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_Head", "float_Head.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_L_Arm", "float_L_Arm.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_R_Arm", "float_R_Arm.obj"));
	models_.push_back(std::make_unique<Model>("Resources/weapon", "weapon.obj"));

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

	models_[kModelIndexWeapon]->transform_.parent_ = &models_[kModelIndexHead]->transform_;

	behaviorRequest_ = Behavior::kRoot;

	velocity_ = {};

	floatingParameter_ = 0.0f;

	InitializeFloatingGimmick();

	isJump_ = false;

	isOnFloor_ = true;

	isNowOnFloor_ = true;
}

void Player::SetGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


}

void Player::ApplyGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


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
	transform_.SetWorldTranslateParent(transform);
}

void Player::ClearWorldTranslateParent()
{
	transform_.ClearWorldTranslateParent();
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

	Input* input = Input::GetInstance();

	const float speed = 1.0f;

	Vector2 inputVector = input->GetGamePadLStick();

	Vector3 move = { inputVector.x, 0.0f,inputVector.y };

	move = move.Normalize() * speed;

	move = move * Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_);

	if (input->PressedGamePadButton(Input::GamePadButton::kA) && !isJump_) {
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
		transform_.rotate_.y = camera_->transform_.rotate_.y;
		transform_.isUseOtherRotateMat_ = false;
	}
	else {

		Vector3 rotate = Vector3{ velocity_.x,0.0f,velocity_.z }.Normalize();
		Vector3 pos = Vector3{ camera_->transform_.worldPos_.x,0.0f,camera_->transform_.worldPos_.z };
		Vector3 pos1 = Vector3{ transform_.worldPos_.x,0.0f,transform_.worldPos_.z };
		Vector3 rotate1 = Vector3{ pos - pos1 }.Normalize();

		transform_.SetOtherRotateMatrix(rotate1, rotate, Matrix4x4::MakeRotateYMatrix(camera_->transform_.rotate_.y));
	}

	if (input->PressedGamePadButton(Input::GamePadButton::kB) && !isJump_) {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackInitialize() { behaviorAttackRequest_ = BehaviorAttack::kExtra; }

void Player::EaseVectorClear() {
	easeStartPos_.clear();
	easeEndPos_.clear();
	easeStartRot_.clear();
	easeEndRot_.clear();
}

void Player::AttackBehaviorExtraInitialize() {

	EaseVectorClear();

	count_ = 0;

	const float pi = std::numbers::pi_v<float>;

	easeStartPos_.push_back(models_[kModelIndexBody]->transform_.translate_);
	easeEndPos_.push_back({ 0.0f, 0.0f, 0.0f });
	easeStartRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);
	easeEndRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);

	easeStartRot_.push_back(models_[kModelIndexL_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });

	easeStartRot_.push_back(models_[kModelIndexR_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });
}

void Player::AttackBehaviorExtraUpdate() {

	const int maxFrame = 5;
	count_++;

	models_[kModelIndexBody]->transform_.translate_ = Ease::UseEase(
		easeStartPos_[kBody], easeEndPos_[kBody], count_, maxFrame, Ease::Constant);
	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant);

	if (count_ == maxFrame) {
		behaviorAttackRequest_ = BehaviorAttack::kExtra2;
		count_ = 0;
	}
}

void Player::AttackBehaviorExtra2Initialize() {
	EaseVectorClear();

	count_ = 0;

	const float pi = std::numbers::pi_v<float>;

	easeStartRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);
	easeEndRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);

	easeStartRot_.push_back(models_[kModelIndexL_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 6.0f + pi, 0.0f, 0.0f });

	easeStartRot_.push_back(models_[kModelIndexR_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 6.0f + pi, 0.0f, 0.0f });

	easeStartRot_.push_back({ -pi / 2.0f, 0.0f, 0.0f });
	easeEndRot_.push_back({ -pi / 6.0f, 0.0f, 0.0f });
}

void Player::AttackBehaviorExtra2Update() {
	const int maxFrame = 15;
	count_++;

	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexWeapon]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kWeapon], easeEndRot_[kWeapon], count_, maxFrame, Ease::Constant);

	if (count_ == maxFrame) {
		behaviorAttackRequest_ = BehaviorAttack::kAttack;
	}
}

void Player::AttackBehaviorAttackInitialize() {
	EaseVectorClear();

	count_ = 0;

	const float pi = std::numbers::pi_v<float>;

	easeStartRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);
	easeEndRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);

	easeStartRot_.push_back(models_[kModelIndexL_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });

	easeStartRot_.push_back(models_[kModelIndexR_arm]->transform_.rotate_);
	easeEndRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });

	easeStartRot_.push_back(models_[kModelIndexWeapon]->transform_.rotate_);
	easeEndRot_.push_back({ -pi / 2.0f, 0.0f, 0.0f });
}

void Player::AttackBehaviorAttackUpdate() {

	const int maxFrame = 10;
	count_++;

	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexWeapon]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kWeapon], easeEndRot_[kWeapon], count_, maxFrame, Ease::Constant);

	if (count_ == maxFrame) {
		behaviorAttackRequest_ = BehaviorAttack::kRigor;
	}
}

void Player::AttackBehaviorRigorInitialize() { count_ = 0; }

void Player::AttackBehaviorRigorUpdate() {

	const int maxFrame = 20;
	count_++;

	if (count_ == maxFrame) {
		behaviorAttackRequest_ = BehaviorAttack::kReturn;
	}
}

void Player::AttackBehaviorReturnInitialize() {
	EaseVectorClear();
	count_ = 0;

	easeStartRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);
	easeEndRot_.push_back(models_[kModelIndexBody]->transform_.rotate_);

	easeStartRot_.push_back(models_[kModelIndexL_arm]->transform_.rotate_);
	easeEndRot_.push_back({ 0.0f, 0.0f, 0.0f });

	easeStartRot_.push_back(models_[kModelIndexR_arm]->transform_.rotate_);
	easeEndRot_.push_back({ 0.0f, 0.0f, 0.0f });
}

void Player::AttackBehaviorReturnUpdate() {

	const int maxFrame = 15;

	count_++;

	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count_, maxFrame, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count_, maxFrame, Ease::Constant);

	if (count_ == maxFrame) {
		behaviorRequest_ = Behavior::kRoot;

	}
}

void Player::BehaviorAttackUpdate() {

	if (behaviorAttackRequest_) {
		// 振る舞いを変更する
		behaviorAttack_ = behaviorAttackRequest_.value();

		// 各振る舞いごとの初期化を実行
		switch (behaviorAttack_) {
		case BehaviorAttack::kExtra:

			AttackBehaviorExtraInitialize();
			break;
		case BehaviorAttack::kExtra2:

			AttackBehaviorExtra2Initialize();
			break;
		case BehaviorAttack::kAttack:

			AttackBehaviorAttackInitialize();
			break;
		case BehaviorAttack::kRigor:

			AttackBehaviorRigorInitialize();
			break;
		case BehaviorAttack::kReturn:

			AttackBehaviorReturnInitialize();
			break;
		default:
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorAttackRequest_ = std::nullopt;
	}

	switch (behaviorAttack_) {
	case BehaviorAttack::kExtra:

		AttackBehaviorExtraUpdate();
		break;
	case BehaviorAttack::kExtra2:

		AttackBehaviorExtra2Update();
		break;
	case BehaviorAttack::kAttack:

		AttackBehaviorAttackUpdate();
		break;
	case BehaviorAttack::kRigor:

		AttackBehaviorRigorUpdate();
		break;
	case BehaviorAttack::kReturn:

		AttackBehaviorReturnUpdate();
		break;
	}
}

void Player::Update() {

	ApplyGlobalVariable();

	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();

			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;

	}

	switch (behavior_) {
	case Behavior::kRoot:
		BehaviorRootUpdate();

		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();

		break;
	}

	if (transform_.worldPos_.y <= -20.0f) {
		Initialize();
	}

	UpdateMat();

	isOnFloor_ = false;
}

void Player::Draw(const Matrix4x4& viewProjection) {

	for (int i = 0; i < kEnd; i++) {

		if (i <= kWeapon) {
			models_[i]->Draw(viewProjection);
		}
		else if (behavior_ == Behavior::kAttack && behaviorAttack_ != BehaviorAttack::kExtra &&
			behaviorAttack_ != BehaviorAttack::kReturn) {
			models_[i]->Draw(viewProjection);
		}

	}
}