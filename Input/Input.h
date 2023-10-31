#pragma once

#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#include <XInput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")
#include <wrl.h>

#include <vector>
#include "../Utils/Math/Vector2.h"
#include "../Kyoko/WinApp/WinApp.h"

class Input
{
public:

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber) const;

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber) const;

	Vector2 GetGamePadLStick();

	Vector2 GetGamePadRStick();

	bool GetGamePadButtonA();

private:
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

private:
	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> devKeyboard_;
	ComPtr<IDirectInputDevice8> devMouse_;
	
	XINPUT_STATE xInputState_ = {};
	XINPUT_STATE preXInputState_ = {};


	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};

	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	Vector2 mousePosition_;

};

