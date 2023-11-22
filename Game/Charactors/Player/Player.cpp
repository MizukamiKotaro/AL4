#include "Player.h"
#include "CollisionSystem/CollisionConfig/CollisionConfig.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "Input/Input.h"
#include <cmath>

Player::Player()
{
	pos_ = { 640.0f,360.0f };
	radius_ = 15.0f;
	velocity_ = {};
	color_ = 0xffffccff;

}

void Player::Init()
{
	pos_ = { 640.0f,360.0f };
	circle_.SetPos(pos_);
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	Vector2 move = {};

	if (input->PressingKey(DIK_A)) {
		move.x--;
	}
	if (input->PressingKey(DIK_D)) {
		move.x++;
	}
	if (input->PressingKey(DIK_W)) {
		move.y--;
	}
	if (input->PressingKey(DIK_S)) {
		move.y++;
	}

	float speed = 5.0f;

	if (move.x == 0 || move.y == 0) {
		velocity_.x = move.x * speed;
		velocity_.y = move.y * speed;
	}
	else {
		velocity_.x = move.x * speed / std::sqrtf(2.0f);
		velocity_.y = move.y * speed / std::sqrtf(2.0f);
	}

	pos_.x += velocity_.x;
	pos_.y += velocity_.y;

	
}

void Player::Draw()
{
	
}

void Player::OnCollision()
{
	Init();
}
