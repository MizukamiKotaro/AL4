#pragma once

#include <Windows.h>
#include <cstdint>

class WinApp
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int kWindowWidth = 1280; // 横幅
	static const int kWindowHeight = 720; // 縦幅

public: // 静的メンバ関数

	static WinApp* GetInstance();

	//ウィンドウプロシャープ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	void CreateGameWindow();

	// メッセージ処理
	bool ProcessMessage();

	void Finalize();

	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return wndClass_.hInstance; }

private: // メンバ関数
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private: // メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASS wndClass_{}; // ウィンドウクラス
};

