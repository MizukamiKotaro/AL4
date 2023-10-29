#include "Input.h"
#include <cassert>

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

void Input::Initialize(WinApp* winApp) {

	assert(winApp);
	winApp_ = winApp;

	HRESULT hr = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(hr));

	hr = dInput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	assert(SUCCEEDED(hr));

	hr = devKeyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = devKeyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}

void Input::Update() {

	devKeyboard_->Acquire();

	memcpy(keyPre_, key_, sizeof(key_));

	devKeyboard_->GetDeviceState(sizeof(key_), key_);
	
	preXInputState_ = xInputState_;
	XInputGetState(0, &xInputState_);

}

bool Input::PushKey(BYTE keyNumber) const {
	if (key_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) const {
	if (!keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}
	return false;
}

Vector2 Input::GetGamePadLStick() 
{
	if ((xInputState_.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(xInputState_.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		xInputState_.Gamepad.sThumbLX = 0;
		xInputState_.Gamepad.sThumbLY = 0;
	}

	return Vector2(static_cast<float>(xInputState_.Gamepad.sThumbLX) / SHRT_MAX, static_cast<float>(xInputState_.Gamepad.sThumbLY) / SHRT_MAX);
}

Vector2 Input::GetGamePadRStick()
{
	if ((xInputState_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(xInputState_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		xInputState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		xInputState_.Gamepad.sThumbRX = 0;
		xInputState_.Gamepad.sThumbRY = 0;
	}

	return Vector2(static_cast<float>(xInputState_.Gamepad.sThumbRX) / SHRT_MAX, static_cast<float>(xInputState_.Gamepad.sThumbRY) / SHRT_MAX);
}

bool Input::GetGamePadButtonA()
{
	if (xInputState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		return true;
	}

	return false;
}
