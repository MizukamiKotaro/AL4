#pragma once
#include "Utils/Math/Vector2.h"
#include "CollisionSystem/BaseColliderShapeType/BaseColliderShapeType.h"
#include <stdint.h>

class Collider
{
public:

	enum class ColliderType
	{
		UNKNOWN = 0, // 何もなし
		RIGID_BODY, // 体あり。これ同士はめり込み処理あり
		COLLIDER, // 体なし。あたり判定のみ
	};

	struct EditInfo
	{
		enum EditEnumV2 {
			V2POS,
			V2VELOCITY,
			V2COUNT,
		};

		std::vector<std::vector<int>> dAInfo_;

		std::vector<bool> bInfo_;
		std::vector<int> iInfo_;
		std::vector<float> fInfo_;
		std::vector<Vector2> v2Info_;
		std::vector<Vector3> v3Info_;
	};

	struct ShapeCircle {
		Vector2 pos_;
		float radius_;

		void SetInfo(const Vector2& pos, float radius);
		void SetPos(const Vector2& pos);
		void SetRadius(float radius);
	};


	void CreateShape(BaseColliderShapeType::ShapeType shapeType, ColliderType type = ColliderType::UNKNOWN);

	void InitCircle(const Vector2& pos, float radius);

	virtual void OnCollision();

	uint32_t GetCollisionAttribute() const;

	void SetCollisionAttribute(uint32_t attribute);

	uint32_t GetCollisionMask() const;

	void SetCollisionMask(uint32_t mask);

	BaseColliderShapeType::ShapeType GetShapeType() const;

	ShapeCircle GetCircle_() const;

	ColliderType GetColliderType() const;

	void SetColliderType(ColliderType type);

protected:

	ShapeCircle circle_;

	BaseColliderShapeType* shape_;

	ColliderType colliderType_;

private:

	uint32_t collisionAttribute_ = 0x00000000;

	uint32_t collisionMask_ = 0x00000000;

};

