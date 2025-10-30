#include "Enemy.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../Player/Player.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
	VECTOR3 size = { 50, 200, 50 }; // hitModel‚ÌƒTƒCƒY
	VECTOR3 headshotPos = { 0, 170, 0 };
	float headshotR = 25;
	int headshotBonus = 2;
}

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hp_ = hp;

	isAlive_ = true;
	isAttacked_ = false;

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
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
	}
	if (hitModel_ > 0)
	{
		MV1DeleteModel(hitModel_);
	}
}

void Enemy::Update()
{
	if (isAlive_ == false)
	{
		DestroyMe();
	}

	// ˆÚ“®ˆ—
	



	// UŒ‚‚³‚ê‚é

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
	VECTOR3 checkPos1 = VECTOR3(-ENEMY::size.x / 2, 0, -ENEMY::size.z / 2) + transform_.position_;
	VECTOR3 checkPos2 = VECTOR3(ENEMY::size.x / 2, ENEMY::size.y, ENEMY::size.z / 2) + transform_.position_;
	// ‹ŠE‚É“ü‚Á‚Ä‚¢‚é‚Í•`‰æ‚·‚é 
	// ¦CheckCameraViewClip_Box¨‹ŠE‚É“ü‚Á‚Ä‚¢‚é:false
	if (CheckCameraViewClip_Box(checkPos1, checkPos2) == FALSE)
	{
		if (isAlive_ == true)
		{
			Object3D::Draw();
		}
	}
}
