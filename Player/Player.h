#pragma once
#include "../Kyoko/Model.h"
#include "../Transform/Transform.h"
#include <memory>
#include <vector>
#include <optional>

class Camera;

class Player {
public:

	Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// <summary>
	/// 描画
	/// </summary>
	void Draw(const Matrix4x4& viewProjection);

	//const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	const Transform& GetTransform() { return transform_; }

	void SetViewProjection(const Camera* camera) { camera_ = camera; }

	//void ApplyGlobalVariable();

	void SetWorldTranslateParent(const Transform* transform);

	void ClearWorldTranslateParent();

	void EditWorldTransform(const Transform* transform);

	void Collision(const Transform* transform);

private:

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	void BehaviorRootInitialize();

	void BehaviorRootUpdate();

	enum class Behavior {
		kRoot, // 通常状態
	};

	void UpdateMat();

private:
	
	enum Joints {
		kModelIndexBody,
		kModelIndexHead,
		kModelIndexL_arm,
		kModelIndexR_arm,
	};

	Transform transform_;

	const Camera* camera_;

	std::vector<std::unique_ptr<Model>> models_;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	Vector3 velocity_;

	float floatingParameter_;

	bool isJump_;

	bool isOnFloor_;

	bool isNowOnFloor_;

};
