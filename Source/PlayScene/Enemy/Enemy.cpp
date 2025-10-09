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

	hitModel_ = MV1LoadModel("data/model/enemy01_c.mv1");
	assert(hitModel_ > 0);

	transform_.MakeLocalMatrix();

	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());

	MV1SetupCollInfo(hitModel_);
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);

	stage_ = FindGameObject<Stage>();
	time_ = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
	stage_->SetOnGround(transform_.position_, time_, ENEMY::G);
}

void Enemy::Draw()
{
	Object3D::Draw();
}
