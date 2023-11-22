#pragma once
#include "ModelCommon/Model/Model.h"
#include "Utils/Transform/Transform.h"
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include "Utils/Shape/OBB.h"
#include <array>

class Camera;

class Player {
public:

	// 攻撃用定数
	struct ConstAttack {
		// 振りかぶりの時間
		uint32_t aticipationTime;
		// ための時間
		uint32_t chargeTIme;
		// 攻撃振りの時間
		uint32_t swingTime;
		// 硬直時間
		uint32_t recoveryTime;
		// 振りかぶりの移動速さ
		float anticipationSpeed;
		// ための移動速さ
		float chageSpeed;
		// 攻撃振りかぶりの移動速さ
		float swingSpeed;
	};

	static const int ComboNum = 3;

	static const std::array<ConstAttack, ComboNum> kConstAttacks_;

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

	void Collision(const OBB& obb, bool* flag);

	bool GetIsDie() { return isDie_; }

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

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();

	enum class Behavior {
		kRoot, // 通常状態
		kAttack, // 攻撃中
		kDash, // ダッシュ
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

	static void (Player::* spStateInitFuncTable[])();

	static void (Player::* spStateUpdateFuncTable[])();

	static void (Player::* spAttackInitFuncTable[])();

	static void (Player::* spAttackUpdateFuncTable[])();

	void Attack1Init();

	void Attack1Update();


private:
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用媒介変数
		uint32_t dashParameter_;
		uint32_t behaviorDashTime_;
		float dashSpeed_;
	};

	struct WorkAttack {
		uint32_t attackParameter = 0;
		uint32_t comboIndex = 0;
		uint32_t inComboPhase = 0;
		bool comboNext = false;
	};

	WorkAttack workAttack_;

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

	WorkDash workDash_;

	OBB weaponOBB_;

	std::unique_ptr<Model> cube_;

	bool isDie_;

};
