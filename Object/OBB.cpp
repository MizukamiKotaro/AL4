#include "OBB.h"

void OBB::SetOrientations(const Matrix4x4& rotateMat) {
	for (int i = 0; i < 3; i++) {
		orientations[i].x = rotateMat.m[i][0];
		orientations[i].y = rotateMat.m[i][1];
		orientations[i].z = rotateMat.m[i][2];
	}

}

//void OBB::Draw(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//
//	Vector3 verteces[8] = {
//		{-size.x,size.y,-size.z},
//		{size.x,size.y,-size.z},
//		{size.x,size.y,size.z},
//		{-size.x,size.y,size.z},
//		{-size.x,-size.y,-size.z},
//		{size.x,-size.y,-size.z},
//		{size.x,-size.y,size.z},
//		{-size.x,-size.y,size.z},
//	};
//
//	
//	
//	
//	
//	
//	worldMat = {
//		orientations[0].x,orientations[0].y,orientations[0].z,0,
//		orientations[1].x,orientations[1].y,orientations[1].z,0,
//		orientations[2].x,orientations[2].y,orientations[2].z,0,
//		center.x,center.y,center.z,1
//	};
//
//	Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMat, viewProjectionMatrix);
//	Vector3 screen[8] = {};
//
//	for (int i = 0; i < 8; i++) {
//		screen[i] = Matrix4x4::Transform(Matrix4x4::Transform(verteces[i], worldViewProjectionMatrix), viewportMatrix);
//	}
//
//	Novice::DrawLine(int(screen[2].x), int(screen[2].y), int(screen[1].x), int(screen[1].y), color);
//	Novice::DrawLine(int(screen[0].x), int(screen[0].y), int(screen[3].x), int(screen[3].y), color);
//	Novice::DrawLine(int(screen[3].x), int(screen[3].y), int(screen[2].x), int(screen[2].y), color);
//	Novice::DrawLine(int(screen[0].x), int(screen[0].y), int(screen[1].x), int(screen[1].y), color);
//
//	Novice::DrawLine(int(screen[5].x), int(screen[5].y), int(screen[6].x), int(screen[6].y), color);
//	Novice::DrawLine(int(screen[5].x), int(screen[5].y), int(screen[4].x), int(screen[4].y), color);
//	Novice::DrawLine(int(screen[6].x), int(screen[6].y), int(screen[7].x), int(screen[7].y), color);
//	Novice::DrawLine(int(screen[4].x), int(screen[4].y), int(screen[7].x), int(screen[7].y), color);
//
//	Novice::DrawLine(int(screen[0].x), int(screen[0].y), int(screen[4].x), int(screen[4].y), color);
//	Novice::DrawLine(int(screen[1].x), int(screen[1].y), int(screen[5].x), int(screen[5].y), color);
//	Novice::DrawLine(int(screen[2].x), int(screen[2].y), int(screen[6].x), int(screen[6].y), color);
//	Novice::DrawLine(int(screen[3].x), int(screen[3].y), int(screen[7].x), int(screen[7].y), color);
//}