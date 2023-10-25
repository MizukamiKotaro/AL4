#include "Camera.h"
#include "../Kyoko/WinApp/WinApp.h"

Camera::Camera()
{
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	pos_ = { 0.0f,0.0f,-15.0f };

	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, pos_);
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 100.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

}

void Camera::Update()
{
	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, pos_);
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;
}
