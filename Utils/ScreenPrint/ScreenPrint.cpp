#include "ScreenPrint.h"
#include "Externals/imgui/imgui.h"

void ScreenPrint(const char* comment, const Matrix4x4& mat)
{
	ImGui::Begin("PrintMat4x4");
	ImGui::Text(comment);
	ImGui::Text("%6.3f  %6.3f  %6.3f  %6.3f", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
	ImGui::Text("%6.3f  %6.3f  %6.3f  %6.3f", mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
	ImGui::Text("%6.3f  %6.3f  %6.3f  %6.3f", mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
	ImGui::Text("%6.3f  %6.3f  %6.3f  %6.3f", mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);
	ImGui::End();
}
