#include "Input.h"

#include <cassert>

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp* winApp) {

	assert(winApp);
	winApp_ = winApp;

	HRESULT hr = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&dInput_, nullptr);
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