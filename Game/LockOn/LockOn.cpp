#include "LockOn.h"
#include <cassert>
#include <cmath>
#include <numbers>
#include "Input/Input.h"
#include "Game/Enemy/Enemy.h"
#include "Utils/Math/calc.h"

LockOn::LockOn()
{
	model_ = std::make_unique<Model>("Resources/Cube", "Cube.obj");

	parameter_ = 0.0f;
}

void LockOn::Initialize() {

	transform_ = Transform();
	transform_.translate_.z = 130.0f;
	transform_.translate_.x = 20.0f;

	model_->transform_.parent_ = &transform_;
	model_->transform_.translate_.y = 2.0f;
	
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera) {

	Input* input = Input::GetInstance();

	if (target_) {
		if (input->PressedGamePadButton(Input::GamePadButton::LEFT_SHOULDER)) {
			target_ = nullptr;
		}
		else if (!IsWithinTarget(camera)) {
			target_ = nullptr;
		}
	}
	else {
		if (input->PressedGamePadButton(Input::GamePadButton::LEFT_SHOULDER)) {
			SearchTarget(enemies, camera);
		}
	}

	if (target_) {

	}

	transform_.UpdateMatrix();
	model_->Update();
	
}

void LockOn::Draw(const Matrix4x4& viewProjection)
{
	if (target_) {
		model_->Draw(viewProjection);
	}
}

void LockOn::SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// カメラからの距離と敵
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		if (!enemy->GetIsDie()) {
			Vector3 worldPos = enemy->GetCenterPos();

			// ワールドからビュー座標変換
			Vector3 viewPos = worldPos * camera.GetViewMat();

			if (minDistance_ <= viewPos.z && viewPos.z <= maxDistance_) {

				// あくたんを使わない
				// sinの大きさを比較する
				Vector3 u = {viewPos.x,0.0f,viewPos.z};
				u = u.Normalize();
				Vector3 v = {0.0f,0.0f,viewPos.z};
				v = v.Normalize();
				float sinf = Calc::Cross(u, v).Length();
				// これでできてればいいな
				if (sinf <= sinAngle_) {
					targets.emplace_back(std::make_pair(viewPos.z, enemy.get()));
				}
			}
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;
	if (targets.size() != 0) {
		// 距離で昇順にソート
		targets.sort([](std::pair<float, const Enemy*>& pair1, std::pair<float, const Enemy*>& pair2)
			{return pair1.first < pair2.first; });
		target_ = targets.front().second;
	}


}

bool LockOn::IsWithinTarget(const Camera& camera)
{
	if (!target_->GetIsDie()) {
		Vector3 worldPos = target_->GetCenterPos();

		// ワールドからビュー座標変換
		Vector3 viewPos = worldPos * camera.GetViewMat();

		if (minDistance_ <= viewPos.z && viewPos.z <= maxDistance_) {

			// あくたんを使わない
			// sinの大きさを比較する
			Vector3 u = { viewPos.x,0.0f,viewPos.z };
			u = u.Normalize();
			Vector3 v = { 0.0f,0.0f,viewPos.z };
			v = v.Normalize();
			float sinf = Calc::Cross(u, v).Length();

			// これでできてればいいな
			if (sinf <= sinAngle_) {
				return true;
			}
		}
	}

	return false;
}
