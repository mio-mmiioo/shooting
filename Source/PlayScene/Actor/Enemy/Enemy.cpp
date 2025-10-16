#include "Enemy.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../Player/Player.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
	VECTOR3 size = { 50, 200, 50 }; // hitModelのサイズ
	VECTOR3 headshotPos = { 0, 170, 0 };
	float headshotR = 25;
	int headshotBonus = 2;
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

	player_ = FindGameObject<Player>();
	VECTOR3 hit;
	if (Object3D::CollideLine(player_->GetStartPos(), player_->GetWPointerPos(), &hit)) // 銃の軌道上にあるか
	{
		// ヘッドショットになる場合黒丸を表示
		//if (Segment_Point_MinLength(player_->GetStartPos(), player_->GetWPointerPos(), transform_.position_ + ENEMY::headshotPos) < ENEMY::headshotR) // クリティカル内か
		//{
		//	DrawSphere3D(hit, 10, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), TRUE);
		//}

		if (player_->Attack() > 0) // プレイヤーが発砲
		{
			// ヘッドショットっだったらheadshotBonusをかける
			if (Segment_Point_MinLength(player_->GetStartPos(), player_->GetWPointerPos(), transform_.position_ + ENEMY::headshotPos) < ENEMY::headshotR) // クリティカル内か
			{
				hp_ -= player_->Attack() * ENEMY::headshotBonus;
			}
			else
			{
				hp_ -= player_->Attack(); // 攻撃される
			}
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
	VECTOR3 checkPos1 = VECTOR3(-ENEMY::size.x / 2, 0, -ENEMY::size.z / 2) + transform_.position_;
	VECTOR3 checkPos2 = VECTOR3(ENEMY::size.x / 2, ENEMY::size.y, ENEMY::size.z / 2) + transform_.position_;
	// 視界に入っている時は描画する 
	// ※CheckCameraViewClip_Box→視界に入っている:false
	if (CheckCameraViewClip_Box(checkPos1, checkPos2) == FALSE)
	{
		if (isAlive_ == true)
		{
			Object3D::Draw();
		}
	}
}

bool Enemy::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit) const
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	VECTOR3 ret;
	if (Actor::CollideLine(pos1, pos2, &ret))
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
