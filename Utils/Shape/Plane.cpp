#include "Plane.h"

void Plane::Initialize() {

}

void Plane::Update() {

}

//void Plane::Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//	Vector3 center = normal;
//	center *=distance;
//
//	
//	perpendiculars[4];
//
//	perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(normal));
//	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
//	perpendiculars[2] = Calc::Cross(normal, perpendiculars[0]);
//	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };
//
//	Vector3 points[4];
//	for (int32_t index = 0; index < 4; index++) {
//		Vector3 extend = perpendiculars[index] * 2.0f;
//		Vector3 point = center + extend;
//		points[index] = 
//			
//			
//			::Transform(Matrix4x4::Transform(point, viewProjectionMatrix), viewportMatrix);
//	}
//
//	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
//	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
//	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
//	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
//
//}