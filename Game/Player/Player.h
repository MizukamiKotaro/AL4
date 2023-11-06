#pragma once
#include "../../ModelCommon/Model/Model.h"
#include "../../Utils/Transform/Transform.h"
#include <memory>
#include <vector>
#include <optional>
#include <string>

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

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	void BehaviorRootInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();

	void BehaviorAttackUpdate();

	void AttackBehaviorExtraInitialize();

	void AttackBehaviorExtra2Initialize();

	void AttackBehaviorAttackInitialize();

	void AttackBehaviorRigorInitialize();

	void AttackBehaviorReturnInitialize();

	void AttackBehaviorExtraUpdate();

	void AttackBehaviorExtra2Update();

	void AttackBehaviorAttackUpdate();

	void AttackBehaviorRigorUpdate();

	void AttackBehaviorReturnUpdate();

	enum class Behavior {
		kRoot, // 通常状態
		kAttack, // 攻撃中
	};

	enum class BehaviorAttack {
		kExtra, // 予備動作
		kExtra2,  // 予備動作2
		kAttack, // 攻撃
		kRigor, // 硬直
		kReturn, // 姿勢を戻す
	};

	void EaseVectorClear();

	void UpdateMat();

private:

	const std::string groupName_ = "Player";
	
	enum Joints {
		kModelIndexBody,
		kModelIndexHead,
		kModelIndexL_arm,
		kModelIndexR_arm,
		kModelIndexWeapon,
		kEnd,
	};

	Transform transform_;

	const Camera* camera_;

	std::vector<std::unique_ptr<Model>> models_;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	BehaviorAttack behaviorAttack_ = BehaviorAttack::kExtra;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 次の振る舞いリクエスト
	std::optional<BehaviorAttack> behaviorAttackRequest_ = std::nullopt;

	// イージング用
	std::vector<Vector3> easeStartPos_;
	std::vector<Vector3> easeEndPos_;
	std::vector<Vector3> easeStartRot_;
	std::vector<Vector3> easeEndRot_;
	int count_ = 0;

	enum EaseNum {
		kBody,
		kLArm,
		kRArm,
		kWeapon
	};

	Vector3 velocity_;

	float floatingParameter_;

	bool isJump_;

	bool isOnFloor_;

	bool isNowOnFloor_;

};
