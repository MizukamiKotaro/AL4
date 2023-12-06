#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "Utils/Math/Quaternion.h"

void ScreenPrint(const char* comment, const Matrix4x4& mat);

void ScreenPrint(const char* comment, const Quaternion& qua);

void ScreenPrint(const char* comment, const Vector3& value);

void ScreenPrint(const char* comment, const float& value);