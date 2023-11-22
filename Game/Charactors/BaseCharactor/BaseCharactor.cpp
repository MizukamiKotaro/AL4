#include "BaseCharactor.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"

void BaseCharactor::Init()
{
}

void BaseCharactor::Update()
{
	
	CollisionManager::GetInstance()->SetCollider(this);
}

void BaseCharactor::OnCollision() {

}

void BaseCharactor::Draw()
{

}
