#include "Enemy.h"
#include "CollisionSystem/CollisionConfig/CollisionConfig.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include <cmath>

Enemy::Enemy()
{
	pos_ = { 640.0f,100.0f };
	radius_ = 30.0f;
	velocity_ = { 5.0f,0.0f };
	color_ = 0x77ffffff;

	InitCircle(pos_, radius_);
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);

	isDead_ = false;
}

void Enemy::Init()
{
	pos_ = { 640.0f,100.0f };
	velocity_ = { 5.0f,0.0f };

	isDead_ = false;
}

void Enemy::Update()
{
	pos_.x += velocity_.x;

	if (pos_.x + radius_ >= 1280) {
		pos_.x = 1280 - radius_;
		velocity_.x *= -1;
	}
	else if (pos_.x - radius_ <= 0) {
		pos_.x = radius_;
		velocity_.x *= -1;
	}

	circle_.SetPos(pos_);
	CollisionManager::GetInstance()->SetCollider(this);
}

void Enemy::OnCollision()
{
	isDead_ = true;
}

bool Enemy::GetIsDead() const
{
	return isDead_;
}
