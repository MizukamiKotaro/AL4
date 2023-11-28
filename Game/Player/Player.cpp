#include "Player.h"
#include <cassert>
#include "Utils/Ease/Ease.h"
#include <cmath>
#include "Utils/Camera/Camera.h"
#include "Input/Input.h"
#include <numbers>
#include "GlobalVariables/GlobalVariables.h"
#include "Utils/Math/calc.h"
#include "Utils/Collision/Collision.h"
#include "Game/LockOn/LockOn.h"
#include <algorithm>
#include "Game/Enemy/Enemy.h"
#include "Particle/Particle.h"

const std::array<Player::ConstAttack, Player::ComboNum> Player::kConstAttacks_ =
{
	{
		// 振りかぶり、攻撃前硬直、攻撃振り時間、硬直<frame>各フェーズの移動速さ
		{10,5,10,0,0.2f,0.0f,0.0f},
		{15,20,15,0,0.3f,0.0f,0.0f},
		{10,30,5,30,0.4f,0.0f,0.0f},
	}
};


Player::Player()
{
	models_.push_back(std::make_unique<Model>("Resources/float_Body", "float_Body.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_Head", "float_Head.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_L_Arm", "float_L_Arm.obj"));
	models_.push_back(std::make_unique<Model>("Resources/float_R_Arm", "float_R_Arm.obj"));
	models_.push_back(std::make_unique<Model>("Resources/weapon", "weapon.obj"));

	cube_ = std::make_unique<Model>("Resources/Cube", "Cube.obj");

	camera_ = nullptr;

	isNowOnFloor_ = false;
	isOnFloor_ = false;
	isJump_ = false;
	velocity_ = {};
	floatingParameter_ = 0.0f;

	workDash_.dashParameter_ = 0;
	workDash_.behaviorDashTime_ = 5;
	workDash_.dashSpeed_ = 0.2f;

	SetGlobalVariable();

	weaponOBB_.size = { 1.0f,1.5f,1.0f };
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
	cube_->transform_.parent_ = &models_[kModelIndexWeapon]->transform_;

	models_[kModelIndexL_arm]->transform_.rotate_ = {};
	models_[kModelIndexR_arm]->transform_.rotate_ = {};

	behaviorRequest_ = Behavior::kRoot;

	velocity_ = {};

	floatingParameter_ = 0.0f;

	InitializeFloatingGimmick();

	isJump_ = false;

	isOnFloor_ = true;

	isNowOnFloor_ = true;

	isDie_ = false;
	
	UpdateMat();

	workAttack_ = WorkAttack();
}

void Player::SetGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup(groupName_);

	globalVariables->AddItem(groupName_, "behaviorDashTime", static_cast<int>(workDash_.behaviorDashTime_));
	globalVariables->AddItem(groupName_, "dashSpeed", workDash_.dashSpeed_);

	globalVariables->AddItem(groupName_, "OBBLocalPos", cube_->transform_.translate_);
	globalVariables->AddItem(groupName_, "OBBScale", cube_->transform_.scale_);

	ApplyGlobalVariable();
}

void Player::ApplyGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	workDash_.behaviorDashTime_ = globalVariables->GetIntValue(groupName_, "behaviorDashTime");
	workDash_.dashSpeed_ = globalVariables->GetFloatValue(groupName_, "dashSpeed");

	cube_->transform_.translate_ = globalVariables->GetVector3Value(groupName_, "OBBLocalPos");
	cube_->transform_.scale_ = globalVariables->GetVector3Value(groupName_, "OBBScale");
}

void Player::UpdateMat()
{
	transform_.UpdateMatrix();

	for (std::unique_ptr<Model>& model : models_) {
		model->Update();
	}
	cube_->Update();

	weaponOBB_.center = cube_->transform_.worldPos_;
	weaponOBB_.size = cube_->transform_.scale_;
	weaponOBB_.SetOrientations(models_[Joints::kModelIndexWeapon]->transform_.worldMat_);
}

void Player::Attack1Init()
{

}

void Player::Attack1Update()
{
	


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
		isDie_ = true;
		/*Initialize();
		UpdateMat();*/
	}

}

void Player::Collision(const OBB& obb, bool* flag)
{

	if (behavior_ == Behavior::kAttack) {
		if (Collision::IsCollision(weaponOBB_, obb)) {
			*flag = true;
		}
	}
}

void Player::Collision(Enemy* enemy, Particle* particle)
{
	if (behavior_ == Behavior::kAttack && behaviorAttack_ == BehaviorAttack::kAttack) {
		if (Collision::IsCollision(weaponOBB_, enemy->GetOBB()) && enemy->life_ != 0) {
			if (enemy->hitCombo_ != workAttack_.comboIndex) {
				enemy->hitCombo_ = workAttack_.comboIndex;
				enemy->life_--;
				enemy->isHit_ = true;
				enemy->count_ = 0;
				enemy->skipVector_ = enemy->GetCenterPos() - transform_.worldPos_;
				particle->Begin(enemy->GetCenterPos());
				if (enemy->life_ == 0) {
					*enemy->GetIsDiePtr() = true;
				}
			}
		}
		else {
			enemy->hitCombo_ = -1;
		}
	}

	if (enemy->life_ != 0 && transform_.IsCollisionXZ(enemy->GetTransform())) {
		isDie_ = true;
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

void Player::BehaviorRootInitialize() { 
	InitializeFloatingGimmick();

	models_[kModelIndexHead]->transform_.translate_.y = 3.0f;
	models_[kModelIndexL_arm]->transform_.translate_ = { 1.0f, 2.5f, 0.0f };
	models_[kModelIndexR_arm]->transform_.translate_ = { -1.0f, 2.5f, 0.0f };
	cube_->transform_.parent_ = &models_[kModelIndexWeapon]->transform_;

	models_[kModelIndexL_arm]->transform_.rotate_ = {};
	models_[kModelIndexR_arm]->transform_.rotate_ = {};
}

void Player::BehaviorRootUpdate() {

	UpdateFloatingGimmick();

	Input* input = Input::GetInstance();

	const float speed = 1.0f;

	Vector2 inputVector = input->GetGamePadLStick();

	Vector3 move = { inputVector.x, 0.0f,inputVector.y };

	move = move.Normalize() * speed;

	move = move * camera_->transform_.GetRotMat();

	if (input->PressedGamePadButton(Input::GamePadButton::A) && !isJump_) {
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

	if (velocity_.x != 0 || velocity_.z != 0) {

		Vector3 rotate = Vector3{ velocity_.x,0.0f,velocity_.z }.Normalize();
		Vector3 pos = Vector3{ camera_->transform_.worldPos_.x,0.0f,camera_->transform_.worldPos_.z };
		Vector3 pos1 = Vector3{ transform_.worldPos_.x,0.0f,transform_.worldPos_.z };
		Vector3 rotate1 = Vector3{ pos - pos1 }.Normalize();

		transform_.SetOtherRotateMatrix(rotate1, rotate, camera_->transform_.GetRotMat(Transform::Y));
	}
	else if (lockOn_ && lockOn_->ExistTarget()) {
		Vector3 rotate = Vector3{ lockOn_->GetTargetPos().x - transform_.GetWorldPosition().x,0.0f,
		lockOn_->GetTargetPos().z - transform_.GetWorldPosition().z }.Normalize();

		transform_.SetOtherRotateMatrix({ 0.0f,0.0f,-1.0f }, rotate);
	}

	if (input->PressedGamePadButton(Input::GamePadButton::B) && !isJump_) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (input->PressedGamePadButton(Input::GamePadButton::X) && !isJump_) {
		behaviorRequest_ = Behavior::kDash;
	}
}

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

	easeStartRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });
	easeEndRot_.push_back({ pi / 6.0f + pi, 0.0f, 0.0f });

	easeStartRot_.push_back({ pi / 2.0f, 0.0f, 0.0f });
	easeEndRot_.push_back({ pi / 6.0f + pi, 0.0f, 0.0f });

	easeStartRot_.push_back({ -pi / 2.0f, 0.0f, 0.0f });
	easeEndRot_.push_back({ -pi / 6.0f, 0.0f, 0.0f });
}

void Player::AttackBehaviorExtra2Update() {
	count_++;

	int count = std::clamp<int>(count_, 0, kConstAttacks_[workAttack_.comboIndex].aticipationTime);

	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count, kConstAttacks_[workAttack_.comboIndex].aticipationTime, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count, kConstAttacks_[workAttack_.comboIndex].aticipationTime, Ease::Constant);
	models_[kModelIndexWeapon]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kWeapon], easeEndRot_[kWeapon], count, kConstAttacks_[workAttack_.comboIndex].aticipationTime, Ease::Constant);

	float distance = Vector3(lockOn_->GetTargetPos() - transform_.GetWorldPosition()).Length();
	// 距離しきい値
	const float threshold = 1.0f;

	float speed = kConstAttacks_[workAttack_.comboIndex].anticipationSpeed;

	if (speed > distance - threshold) {
		speed = distance - threshold;
	}

	Vector3 move = { 0.0f,0.0f,-speed };

	move = move * transform_.GetRotMat();

	velocity_.x = move.x;
	velocity_.z = move.z;

	transform_.translate_ += velocity_;

	if (uint32_t(count_) >= kConstAttacks_[workAttack_.comboIndex].aticipationTime) {
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

	
	count_++;

	int count = std::clamp<int>(count_ - kConstAttacks_[workAttack_.comboIndex].chargeTime, 0, kConstAttacks_[workAttack_.comboIndex].swingTime);

	models_[kModelIndexL_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kLArm], easeEndRot_[kLArm], count, kConstAttacks_[workAttack_.comboIndex].swingTime, Ease::Constant);
	models_[kModelIndexR_arm]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kRArm], easeEndRot_[kRArm], count, kConstAttacks_[workAttack_.comboIndex].swingTime, Ease::Constant);
	models_[kModelIndexWeapon]->transform_.rotate_ = Ease::UseEase(
		easeStartRot_[kWeapon], easeEndRot_[kWeapon], count, kConstAttacks_[workAttack_.comboIndex].swingTime, Ease::Constant);

	if (uint32_t(count) >= kConstAttacks_[workAttack_.comboIndex].swingTime) {
		behaviorAttackRequest_ = BehaviorAttack::kRigor;
	}
}

void Player::AttackBehaviorRigorInitialize() { count_ = 0; }

void Player::AttackBehaviorRigorUpdate() {

	count_++;

	if (uint32_t(count_) >= kConstAttacks_[workAttack_.comboIndex].recoveryTime) {
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

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;

}

void Player::BehaviorDashUpdate()
{

	Vector3 move = { 0.0f, 0.0f,-workDash_.dashSpeed_ };

	if (transform_.isUseOtherRotateMat_) {
		move = move * transform_.otherRotateMat_;

	}
	else {
		move = move * Matrix4x4::MakeRotateYMatrix(transform_.rotate_.y);
	}

	velocity_.x = move.x;
	velocity_.z = move.z;

	transform_.translate_ += velocity_;

	if (++workDash_.dashParameter_ >= workDash_.behaviorDashTime_) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::BehaviorAttackInitialize() { 
	behaviorAttackRequest_ = BehaviorAttack::kExtra2;
	workAttack_.attackParameter = 0;
	workAttack_.comboIndex = 0;
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

	if (lockOn_ && lockOn_->ExistTarget()) {
		Vector3 rotate = Vector3{ lockOn_->GetTargetPos().x - transform_.GetWorldPosition().x,0.0f,
		lockOn_->GetTargetPos().z - transform_.GetWorldPosition().z }.Normalize();

		transform_.SetOtherRotateMatrix({ 0.0f,0.0f,-1.0f }, rotate);
	}


	if (workAttack_.comboIndex < ComboNum - 1) {
		if (Input::GetInstance()->PressedGamePadButton(Input::GamePadButton::B)) {
			workAttack_.comboNext = true;
		}
	}

	if (++workAttack_.attackParameter >=
		kConstAttacks_[workAttack_.comboIndex].aticipationTime + kConstAttacks_[workAttack_.comboIndex].chargeTime +
		kConstAttacks_[workAttack_.comboIndex].recoveryTime + kConstAttacks_[workAttack_.comboIndex].swingTime) {

		if (workAttack_.comboNext) {

			workAttack_.comboNext = false;

			behaviorAttackRequest_ = BehaviorAttack::kExtra2;

			workAttack_.comboIndex++;

			workAttack_.attackParameter = 0;
		}
		else {
			behaviorRequest_ = Behavior::kRoot;
		}
	}
}

void (Player::* Player::spStateInitFuncTable[])() {
	&Player::BehaviorRootInitialize,
	&Player::BehaviorAttackInitialize,
	&Player::BehaviorDashInitialize,
};

void (Player::* Player::spStateUpdateFuncTable[])() {
	&Player::BehaviorRootUpdate,
	&Player::BehaviorAttackUpdate,
	&Player::BehaviorDashUpdate,
};

void Player::Update() {

	ApplyGlobalVariable();

	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		// 各振る舞いごとの初期化を実行
		(this->*spStateInitFuncTable[static_cast<size_t>(behavior_)])();
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;

	}

	(this->*spStateUpdateFuncTable[static_cast<size_t>(behavior_)])();

	if (transform_.worldPos_.y <= -20.0f) {
		//Initialize();
		isDie_ = true;
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
			//cube_->Draw(viewProjection);
		}

	}
}