#pragma once

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include <string>
#include<d3d12.h>

// スプライト共通部分
class SpriteCommon
{
public:

	static SpriteCommon* GetInstance();

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void PreDraw();

public:

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

private:

	ID3D12Device* device_;
	ID3D12GraphicsCommandList* commandList_;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3DBlob> signatureBlob_;
	ComPtr<ID3DBlob> errorBlob_;
	ComPtr<ID3D12RootSignature> rootSignature_;

	ComPtr<IDxcBlob> vertexShaderBlob_;
	ComPtr<IDxcBlob> pixelShaderBlob_;

	ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
	SpriteCommon() = default;
	~SpriteCommon() = default;
	SpriteCommon(const SpriteCommon&) = delete;
	SpriteCommon& operator=(const SpriteCommon&) = delete;

	void InitializeDXC();

	void InitializePSO();

	//コンパイル用関数
	IDxcBlob* CompileShader(
		//compilerするShaderファイルへパス
		const std::wstring& filePath,
		//compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したもの3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};

