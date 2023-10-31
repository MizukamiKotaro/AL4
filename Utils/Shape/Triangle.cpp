#include "Triangle.h"


//void Triangle::Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//
//	Vector3 screenPos[3];
//
//	for (uint32_t index = 0; index < 3; index++) {
//		Matrix4x4 worldMatrix = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, vertices[index]);
//		Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, viewProjectionMatrix);
//		
//		screenPos[index] = Matrix4x4::Transform(Vector3(0.0f, 0.0f, 0.0f), worldViewProjectionMatrix);
//		screenPos[index] = Matrix4x4::Transform(screenPos[index], viewportMatrix);
//	}
//	
//	Novice::DrawLine(int(screenPos[0].x), int(screenPos[0].y), int(screenPos[1].x), int(screenPos[1].y), color);
//	Novice::DrawLine(int(screenPos[0].x), int(screenPos[0].y), int(screenPos[2].x), int(screenPos[2].y), color);
//	Novice::DrawLine(int(screenPos[1].x), int(screenPos[1].y), int(screenPos[2].x), int(screenPos[2].y), color);
//}