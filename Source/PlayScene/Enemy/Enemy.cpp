#include "Enemy.h"
#include <assert.h>
#include "../Map/Stage.h"
#include "../Player/Player.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
}

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hp_ = hp;

	isAlive_ = true;

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
	player_ = FindGameObject<Player>();
	time_ = 0;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (isAlive_ == false)
	{
		DestroyMe();
	}

	player_ = FindGameObject<Player>();
	VECTOR3 hit;
	if (CollideLine(player_->GetTransform().position_, player_->GetWPointerPos(), &hit)) // 銃の軌道上にあるか
	{
		if (player_->IsAttack()) // プレイヤーが発砲
		{
			addHp(-5); // 攻撃される ここに入れる値をプレイヤーからもらいたい
		}
	}

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
	stage_->SetOnGround(transform_.position_, time_, ENEMY::G);
}

void Enemy::Draw()
{
	if (isAlive_ == true)
	{
		Object3D::Draw();
	}
}

bool Enemy::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit) const
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	VECTOR3 ret;
	if (Object3D::CollideLine(pos1, pos2, &ret))
	{
		found = true;
		VECTOR3 v = pos1 - ret;
		float len = v.Size();
		if (len < nowVal)
		{
			nowVal = len;
			now = ret;
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}
	return found;
}
