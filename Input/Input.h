#pragma once

#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#include <XInput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <wrl.h>

#include <vector>
#include "../Math/Vector2.h"
#include "../Kyoko/WinApp/WinApp.h"

class Input
{
public:

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	enum class PadType {
		DirectInput,
		XInput,
	};

	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

	struct Joystick {
		Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
		int32_t deadZoneL_;
		int32_t deadZoneR_;
		PadType type_;
		State state_;
		State statePre_;
	};


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

	/// <summary>
	/// マウスの押下をチェック
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>押されているか</returns>
	bool IsPressMouse(int32_t mouseNumber) const;

	/// <summary>
	/// マウスのトリガーをチェック。押した瞬間だけtrueになる
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>トリガーか</returns>
	bool IsTriggerMouse(int32_t buttonNumber) const;

	/// <summary>
	/// ホイールスクロール量を取得する
	/// </summary>
	/// <returns>ホイールスクロール量。奥側に回したら+。Windowsの設定で逆にしてたら逆</returns>
	int32_t GetWheel() const;

	/// <summary>
	/// マウスの位置を取得する（ウィンドウ座標系）
	/// </summary>
	/// <returns>マウスの位置</returns>
	const Vector2& GetMousePosition() const;


	/// <summary>
	/// 現在のジョイスティック状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">現在のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetJoystickState(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// 前回のジョイスティック状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">前回のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// 現在のジョイスティック状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">現在のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetJoystickState(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// 前回のジョイスティック状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">前回のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// デッドゾーンを設定する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="deadZoneL">デッドゾーン左スティック 0~32768</param>
	/// <param name="deadZoneR">デッドゾーン右スティック 0~32768</param>
	/// <returns>正しく取得できたか</returns>
	void SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR);

	/// <summary>
	/// 接続されているジョイスティック数を取得する
	/// </summary>
	/// <returns>接続されているジョイスティック数</returns>
	size_t GetNumberOfJoysticks();

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
	std::vector<Joystick> devJoysticks_;

	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};

	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	Vector2 mousePosition_;

};

