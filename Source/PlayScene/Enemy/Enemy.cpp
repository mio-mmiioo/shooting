#include "Enemy.h"
#include <assert.h>
#include "../Map/Stage.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
}

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;

	hModel_ = MV1LoadModel("data/model/enemy01.mv1");
	assert(hModel_ > 0);

	stage_ = FindGameObject<Stage>();
	time_ = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	stage_->SetOnGround(transform_.position_, time_, ENEMY::G);
}

//void Enemy::Draw()
//{
//}
