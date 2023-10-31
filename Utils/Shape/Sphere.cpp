#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <cmath>

//Sphere::Sphere(const Vector3& center, const float& radius) {
//	center_ = center;
//	scale_ = { 1.0f,1.0f,1.0f };
//	rotate_ = {};
//	radius_ = radius;
//}
//
//Sphere::Sphere(const 
// 
// 
// & center, const Vector3& rotate, const float& radius) {
//	center_ = center;
//	scale_ = { 1.0f,1.0f,1.0f };
//	rotate_ = rotate;
//	radius_ = radius;
//}
//
//Sphere::Sphere(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius) {
//	center_ = center;
//	scale_ = scale;
//	rotate_ = rotate;
//	radius_ = radius;
//}

void Sphere::Initialize(const Vector3& center, const float& radius) {
	center_ = center;
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = {};
	radius_ = radius;
}

void Sphere::Initialize(const Vector3& center, const Vector3& rotate, const float& radius) {
	center_ = center;
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = rotate;
	radius_ = radius;
}

void Sphere::Initialize(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius) {
	center_ = center;
	scale_ = scale;
	rotate_ = rotate;
	radius_ = radius;
}

//void Sphere::Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//	const uint32_t kSubdivision = 16;
//	const float kLonEvery = static_cast<float> (M_PI / kSubdivision); // 経度分割1つ分の角度
//	const float kLatEvery = static_cast<float> (2.0f * M_PI / kSubdivision); // 井戸分割1つ分の角度
//
//	Matrix4x4 worldMatrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, center_);
//	Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, viewProjectionMatrix);
//
//	//　緯度の方向に分割 -π/2 ～ π/2
//	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
//		float lat = static_cast<float>(- M_PI / 2.0f + kLatEvery * latIndex);
//		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
//			float lon = static_cast<float>(kLonEvery * lonIndex);
//
//			Vector3 a, b, c;
//			a = { std::cos(lat) * std::cos(lon),std::sin(lat),std::cos(lat) * std::sin(lon) };
//			b = { std::cos(lat + kLatEvery) * std::cos(lon),std::sin(lat + kLatEvery),std::cos(lat + kLatEvery) * std::sin(lon) };
//			c = { std::cos(lat) * std::cos(lon + kLonEvery),std::sin(lat),std::cos(lat) * std::sin(lon + kLonEvery) };
//
//			a = a * radius_;
//			b = b * radius_;
//			c = c * radius_;
//
//			Vector3 screenAPosition, screenBPosition, screenCPosition;
//			Vector3 nbcA, nbcB, nbcC;
//
//			nbcA = Matrix4x4::Transform(a, worldViewProjectionMatrix);
//			screenAPosition = Matrix4x4::Transform(nbcA, viewportMatrix);
//			nbcB = Matrix4x4::Transform(b, worldViewProjectionMatrix);
//			screenBPosition = Matrix4x4::Transform(nbcB, viewportMatrix);
//			nbcC = Matrix4x4::Transform(c, worldViewProjectionMatrix);
//			screenCPosition = Matrix4x4::Transform(nbcC, viewportMatrix);
//
//			Novice::DrawLine(static_cast<int>(screenAPosition.x), static_cast<int>(screenAPosition.y), static_cast<int>(screenBPosition.x), static_cast<int>(screenBPosition.y), color);
//			Novice::DrawLine(static_cast<int>(screenAPosition.x), static_cast<int>(screenAPosition.y), static_cast<int>(screenCPosition.x), static_cast<int>(screenCPosition.y), color);
//		}
//	}
//
//}