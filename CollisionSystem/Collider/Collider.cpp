#include "Collider.h"
#include "CollisionSystem/BaseColliderShapeType/BaseColliderShapeType.h"

void Collider::ShapeCircle::SetInfo(const Vector2& pos, float radius)
{
	pos_ = pos;
	radius_ = radius;
}

void Collider::ShapeCircle::SetPos(const Vector2& pos)
{
	pos_ = pos;
}

void Collider::ShapeCircle::SetRadius(float radius)
{
	radius_ = radius;
}

void Collider::CreateShape(BaseColliderShapeType::ShapeType shapeType, ColliderType type)
{


}

void Collider::InitCircle(const Vector2& pos, float radius)
{
	//shapeType_->shapeType_ = BaseColliderShapeType::ShapeType::CIRCLE;

	circle_.pos_ = pos;
	circle_.radius_ = radius;
}

void Collider::OnCollision()
{
}

uint32_t Collider::GetCollisionAttribute() const
{
	return collisionAttribute_;
}

void Collider::SetCollisionAttribute(uint32_t attribute)
{
	collisionAttribute_ = collisionAttribute_ | attribute;
}

uint32_t Collider::GetCollisionMask() const
{
	return collisionMask_;
}

void Collider::SetCollisionMask(uint32_t mask)
{
	collisionMask_ = collisionMask_ | mask;
}

BaseColliderShapeType::ShapeType Collider::GetShapeType() const
{
	return shape_->GetShapeType();
}

Collider::ShapeCircle Collider::GetCircle_() const
{
	return circle_;
}

Collider::ColliderType Collider::GetColliderType() const
{
	return colliderType_;
}

void Collider::SetColliderType(ColliderType type)
{
	colliderType_ = type;
}
