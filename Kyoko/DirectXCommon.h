#pragma once

#include<d3d12.h>
#include<dxgi1_6.h>
#include <wrl.h>
#include "WinApp/WinApp.h"
#include <vector>
#include <chrono>

class DirectXCommon
{
public: // メンバ関数

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	void Finalize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() const { return device_.Get(); }

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

private:

	/// <summary>
	/// FPS固定初期値
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();


private: // メンバ変数

	// ウィンドウズアプリケーション管理
	WinApp* winApp_ = nullptr;

	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIAdapter4> useAdapter_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	ComPtr<ID3D12Resource> depthStencilResource_;
	ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;

	// 記録時間(FPS固定)
	std::chrono::steady_clock::time_point reference_;

private: // メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	void CreateFinalRenderTargets();

	/// <summary>
	/// ディスクリプタヒープの作成関数
	/// </summary>
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device_, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device_, int32_t width, int32_t height);

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();
};