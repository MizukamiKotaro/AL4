#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "../externals/DirectXTex/DirectXTex.h"
#include <vector>

class TextureManager {
public:

	static const size_t kNumDescriptors_ = 256;
	
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static TextureManager* GetInstance();

	void Initialize(ID3D12Device* device);

	void Finalize();

	struct Texture {
		ComPtr<ID3D12Resource> resource_;
		ComPtr<ID3D12Resource> intermediateResource_;
		D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
		D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
		std::string filePath_;
	};

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(const uint32_t& textureHandle)
	{ return textures_[textureHandle].srvGPUDescriptorHandle_; }

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }

	uint32_t LoadTexture(const std::string& filePath);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	DirectX::ScratchImage Load(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	void CreateSRVHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:

	ID3D12Device* device_;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	std::vector<Texture> textures_;

};