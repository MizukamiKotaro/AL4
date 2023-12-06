#include "FollowCamera.h"
#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#include <XInput.h>
#include "Input/Input.h"
#include "Utils/Math/calc.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Game/LockOn/LockOn.h"

FollowCamera::FollowCamera()
{
	
	interTarget_ = {};

	destinationAngleY_ = {};

	interpolationRate_ = 0.05f;

	SetGlobalVariable();

}

void FollowCamera::Initialize() 
{
	camera_.Initialize();
}

void FollowCamera::Update() {

	ApplyGlobalVariable();

	const float rotSpeed = 0.03f;

	destinationAngleY_ -= Input::GetInstance()->GetGamePadRStick().x * rotSpeed;

	if (Input::GetInstance()->PressedGamePadButton(Input::GamePadButton::RIGHT_THUMB)) {

		// プレイヤーのオイラー角を使用していないのに呼び出しているのでバグる
		/*if (target_) {
			
			interTarget_ = target_->worldPos_;
			destinationAngleY_ = target_->rotate_.y;
		}*/
	}

	if (lockOn_ && lockOn_->ExistTarget()) {

		Vector3 rotate = Vector3{ lockOn_->GetTargetPos().x - camera_.transform_.GetWorldPosition().x,0.0f,
			lockOn_->GetTargetPos().z - camera_.transform_.GetWorldPosition().z }.Normalize();

		camera_.transform_.SetOtherRotateMatrix({0.0f,0.0f,1.0f}, rotate, Transform::Y);

		/*camera_.transform_.SetOtherRotMat(Matrix4x4::MakeRotateXMatrix(camera_.transform_.rotate_.x) *
			camera_.transform_.GetOtherRotMat() * Matrix4x4::MakeRotateZMatrix(camera_.transform_.rotate_.z));*/
	}
	else {
		if (camera_.transform_.isUseOtherRotateMat_) {
			camera_.transform_.ClearOtherRotMat();
			destinationAngleY_ = camera_.transform_.rotate_.y;
		}
		else {
			camera_.transform_.rotate_.y = Calc::Lerp(camera_.transform_.rotate_.y, destinationAngleY_, interpolationRate_);
		}
	}
	
	if (target_) {
		interTarget_ = Calc::Lerp(interTarget_, target_->worldPos_, 0.1f);

		camera_.transform_.translate_ = interTarget_ + CalcOffset();
	}

	camera_.Update();

}

void FollowCamera::SetTarget(const Transform* target)
{
	target_ = target;
	Reset();
}

void FollowCamera::SetLockOn(const LockOn* lockOn)
{
	lockOn_ = lockOn;
}

void FollowCamera::Reset()
{

	// 追従対象がいれば
	if (target_) {
		// 追従座標・角度の初期化
		interTarget_ = target_->worldPos_;
		camera_.transform_.rotate_.y = target_->rotate_.y;
	}

	destinationAngleY_ = camera_.transform_.rotate_.y;

	camera_.transform_.translate_ = interTarget_ + CalcOffset();

}

void FollowCamera::SetGlobalVariable()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup(groupName_);

	globalVariables->AddItem(groupName_, "interpolationRate", interpolationRate_);

	ApplyGlobalVariable();
}

void FollowCamera::ApplyGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	interpolationRate_ = globalVariables->GetFloatValue(groupName_, "interpolationRate");

}

Vector3 FollowCamera::CalcOffset() const
{
	Vector3 offset = { 0.0f, 8.0f, -70.0f };

	offset = offset * camera_.transform_.GetRotMat();

	return offset;
}
