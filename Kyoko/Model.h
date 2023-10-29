#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "../Math/Vector2.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4x4.h"
#include "../Transform/Transform.h"
#include "ModelCommon.h"

class Model
{
public:

	Model(const std::string& directoryPath, const std::string& fileName);
	~Model();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

public:

	//void LoadTexture(const std::string& filePath);

private:

	ComPtr<ID3D12Resource> vertexResource_;
	ModelCommon::VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;

	ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_;


public:

	Transform transform_;

private:
	Matrix4x4 uvMatrix_;

	Vector3 uvScale_;
	Vector3 uvRotate_;
	Vector3 uvPos_;
			
	uint32_t modelHundle_;

};

