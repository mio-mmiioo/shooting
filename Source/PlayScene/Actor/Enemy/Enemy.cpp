#include "Enemy.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../../../../Library/Input.h"
#include "../../../ResultScene/Observer.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
	VECTOR3 size = { 50, 200, 50 }; // hitModelのサイズ

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 25.0f;

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

	goPosition_ = VECTOR3(0, 0, 0);
	isArrive_ = true;

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
	state_ = E_STATE::WALK;
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

	Observer::EnemyKilled();
}

void Enemy::Update()
{
	if (isAlive_ == false)
	{
		DestroyMe();
	}

	// 自動移動
	switch (state_) // ステートベースで敵AI
	{
	case E_STATE::WALK:
		UpdateWalk();
		break;
	case E_STATE::STAY:
		UpdateStay();
		break;
	case E_STATE::ATTACK:
		UpdateAttack();
		break;
	}

	// 回転
	{
		//if (Input::IsKeepKeyDown(KEY_INPUT_D))
		//{
		//	transform_.rotation_.y += ENEMY::ROTATE_SPEED * DegToRad;
		//}
		//if (Input::IsKeepKeyDown(KEY_INPUT_A))
		//{
		//	transform_.rotation_.y -= ENEMY::ROTATE_SPEED * DegToRad;
		//}
	}

	// 移動
	{
		//VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
		//velocity = VECTOR3(0, 0, 1) * ENEMY::MOVE_SPEED * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

		//if (Input::IsKeepKeyDown(KEY_INPUT_W))
		//{
		//	transform_.position_ += velocity;
		//}
		//else if (Input::IsKeepKeyDown(KEY_INPUT_S))
		//{
		//	transform_.position_ -= velocity;
		//}
	}

	// 攻撃する

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);

	stage_->SetOnGround(transform_.position_, time_, ENEMY::G); // 重力をかける
	stage_->CheckPush(transform_.position_, transform_.position_ + VECTOR3(0, 0, 1) *  100 * MGetRotY(transform_.rotation_.y), ENEMY::DISTANCE_R); // めり込みを確認する
	stage_->CheckPush(transform_.position_, transform_.position_ + VECTOR3(0, 0, 1) * -100 * MGetRotY(transform_.rotation_.y), ENEMY::DISTANCE_R);
}

void Enemy::Draw()
{
	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = { 0, 180, 0 };
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * 100 * MGetRotY(transform_.rotation_.y), GetColor(255, 255, 255));

	VECTOR3 checkPos1 = VECTOR3(-ENEMY::size.x / 2, 0, -ENEMY::size.z / 2) + transform_.position_;
	VECTOR3 checkPos2 = VECTOR3(ENEMY::size.x / 2, ENEMY::size.y, ENEMY::size.z / 2) + transform_.position_;
	// 視界に入っている時は描画する 
	// ※CheckCameraViewClip_Box→視界に入っている:false
	if (CheckCameraViewClip_Box(checkPos1, checkPos2) == false)
	{
		if (isAlive_ == true)
		{
			Object3D::Draw();
		}
	}
}

void Enemy::UpdateWalk()
{
	// 移動
	{
		if (isArrive_ == false)
		{
			SetMove(goPosition_, 1.0f, 2.0f);

			// 壁があって、まっすぐに進めない、、、


			// 壁がなくてまっすぐに進める
			if (VSize(goPosition_ - transform_.position_) < 200.0f)
			{
				isArrive_ = true;
			}
		}
	}

}

void Enemy::UpdateStay()
{
}

void Enemy::UpdateAttack()
{
}
