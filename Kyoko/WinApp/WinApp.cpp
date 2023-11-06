#include "WinApp.h"
#include "../../Externals/imgui/imgui_impl_win32.h"
#pragma comment(lib, "winmm.lib")
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

void WinApp::CreateGameWindow() {
	//COM(Component Object Model)の初期化

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	//ウィンドウプロシージャ
	wndClass_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名(なんでも良い)
	wndClass_.lpszClassName = L"DirectXGame";
	//インスタンスハンドル
	wndClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wndClass_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kWindowWidth,kWindowHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wndClass_.lpszClassName,      // 利用するクラス名
		L"DirectXGame",	               // タイトルバーの文字(何でもいい)
		WS_OVERLAPPEDWINDOW,   // よく見るウィンドウスタイル
		CW_USEDEFAULT,         // 表示x座標(windowsに任せる)
		CW_USEDEFAULT,         // 表示y座標(windowsに任せる)
		wrc.right - wrc.left,  // ウィンドウ横幅
		wrc.bottom - wrc.top,  // ウィンドウ縦幅
		nullptr,               // 親ウィンドウハンドル
		nullptr,               // メニューハンドル
		wndClass_.hInstance,          // インスタンスハンドル
		nullptr);              // オプション

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinApp::ProcessMessage() {

	MSG msg{};

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize() {
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);
	CloseWindow(hwnd_);
}
