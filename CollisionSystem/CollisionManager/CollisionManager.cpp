#include "CollisionManager.h"
#include <cmath>

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::Init()
{
	Clear();
}

void CollisionManager::Clear()
{
	colliders_.clear();
}

void CollisionManager::SetCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void CollisionManager::CheckCollision()
{

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); itrA++) {
		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); itrB++) {
			Collider* colliderB = *itrB;

			if (IsMatchedConfig(colliderA, colliderB)) {

				if (IsCollision(colliderA,colliderB)) {

					colliderA->OnCollision();

					colliderB->OnCollision();

				}
			}
		}
	}
}

bool CollisionManager::IsMatchedConfig(Collider* a, Collider* b) const
{
	if (((a->GetCollisionAttribute() & b->GetCollisionMask()) >= 0b1) ||
		((b->GetCollisionAttribute() & a->GetCollisionMask()) >= 0b1)) {
		return true;
	}

	return false;
}

bool CollisionManager::IsCollision(Collider* a, Collider* b) const
{

	if (a->GetShapeType() == Collider::ShapeType::CIRCLE && b->GetShapeType() == Collider::ShapeType::CIRCLE) {
		if (IsCollisionCircleCircle(a, b)) {
			return true;
		}
	}

	return false;
}

bool CollisionManager::IsCollisionCircleCircle(Collider* a, Collider* b) const
{

	Collider::ShapeCircle circleA = a->GetCircle_();
	Collider::ShapeCircle circleB = b->GetCircle_();

	float length = std::sqrtf(std::powf(circleA.pos_.x - circleB.pos_.x, 2.0f) + std::powf(circleA.pos_.y - circleB.pos_.y, 2.0f));

	if (length <= circleA.radius_ + circleB.radius_) {
		return true;
	}
		
	return false;
}


