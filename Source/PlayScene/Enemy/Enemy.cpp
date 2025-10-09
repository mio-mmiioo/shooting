#include "Enemy.h"
#include <assert.h>

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;

	hModel_ = MV1LoadModel("data/model/enemy01.mv1");
	assert(hModel_ > 0);
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
}

//void Enemy::Draw()
//{
//}
