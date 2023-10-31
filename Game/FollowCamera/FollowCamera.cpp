#include "FollowCamera.h"
#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#include <XInput.h>
#include "../../Input/Input.h"

FollowCamera::FollowCamera()
{
	
}

void FollowCamera::Initialize() 
{
	camera_.Initialize();
}

void FollowCamera::Update() {

	const float rotSpeed = 0.01f;

	camera_.transform_.rotate_.y -= Input::GetInstance()->GetGamePadRStick().x * rotSpeed;
	
	if (target_) {
		Vector3 offset = { 0.0f, 8.0f, -70.0f };

		offset = offset * Matrix4x4::MakeRotateXYZMatrix(camera_.transform_.rotate_);

		camera_.transform_.translate_ = (target_->worldPos_) + offset;
	}

	camera_.Update();

}