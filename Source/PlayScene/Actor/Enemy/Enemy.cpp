#include "Enemy.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../../../../Library/Input.h"
#include "../../../ResultScene/Observer.h"
#include "../../GameMaster.h"
#include "../../../Sound.h"

namespace ENEMY
{
	VECTOR3 G = { 0.0f, 9.8f, 0.0f };
	VECTOR3 size = { 50.0f, 200.0f, 50.0f }; // hitModelのサイズ

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 50.0f;

	VECTOR3 headshotPos = { 0.0f, 170.0f, 0.0f };
	float headshotR = 25.0f;
	int headshotBonus = 2;

	// 攻撃関連
	const float ATTACK_TIME = 6.0f;
}

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hp_ = hp;

	isAlive_ = true;
	
	goPosition_ = VECTOR3(0.0f, 0.0f, 0.0f);
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
	time_ = 0.0f;
	gravity_ = ENEMY::G;
	distanceR_ = ENEMY::DISTANCE_R;
	moveSpeed_ = ENEMY::MOVE_SPEED;
	rotateSpeed_ = ENEMY::ROTATE_SPEED;
	state_ = E_STATE::WALK;
	
	// 攻撃関連
	attackTimer_ = ENEMY::ATTACK_TIME;
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
		PlaySoundMem(Sound::se["BreakEnemy"], DX_PLAYTYPE_BACK, TRUE);
		Observer::EnemyKilled();
		Observer::AddPoint(100);
		DestroyMe();
	}
	
	//switch (state_) // ステートベースで敵AI
	//{
	//case E_STATE::WALK:
	//	UpdateWalk();
	//	break;
	//case E_STATE::STAY:
	//	UpdateStay();
	//	break;
	//case E_STATE::ATTACK:
	//	UpdateAttack();
	//	break;
	//}

	// 移動 経路探索を使用
	{
		if (isArrive_ == false)
		{
			AutoMove();
		}
		// 手動
		{
			// 移動
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
			// 
			
			// 回転
			//if (Input::IsKeepKeyDown(KEY_INPUT_D))
			//{
			//	transform_.rotation_.y += ENEMY::ROTATE_SPEED * DegToRad;
			//}
			//if (Input::IsKeepKeyDown(KEY_INPUT_A))
			//{
			//	transform_.rotation_.y -= ENEMY::ROTATE_SPEED * DegToRad;
			//}
		}
	}

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	// 攻撃する
	if (GameMaster::IsCanAttackPlayer(this) == true)
	{
		attackTimer_ -= Time::DeltaTime();
		if (attackTimer_ <= 0)
		{
			state_ = E_STATE::ATTACK;
			GameMaster::AttackPlayer(-2);
			attackTimer_ += ENEMY::ATTACK_TIME;
		}
	}
	else
	{
		attackTimer_ = ENEMY::ATTACK_TIME;
	}

	// 位置情報の修正
	GameMaster::CheckSetPosition(transform_, time_, gravity_, distanceR_);

	// 位置情報の更新　これがないと、当たり判定の場所が更新されない
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Enemy::Draw()
{
	DrawPosList(); // 開発時のみ使用

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

void Enemy::SetPosList(std::vector<VECTOR3> posList)
{
	posList_.resize(posList.size() - 1);
	posList_ = posList;
	goPosition_ = posList_[0];
}

void Enemy::UpdateWalk()
{
}

void Enemy::UpdateStay()
{
}

void Enemy::UpdateAttack()
{
}

void Enemy::AutoMove()
{
	if (posList_.size() > 0)
	{
		// 今いる場所はposList_の位置？
		if (VSize(transform_.position_ - goPosition_) > distanceR_) // 中継地に到達していない
		{
			SetMove(goPosition_);
		}
		else // 中継地に到達したので、次の中継地をセット
		{
			goPosition_ = posList_[0];
			posList_.erase(posList_.begin());
		}
	}
	else
	{
		if (VSize(transform_.position_ - goPosition_) > GameMaster::GetDistanceToPlayer(distanceR_))
		{
			SetMove(goPosition_);
		}
		else
		{
			isArrive_ = true;
		}
	}
}

void Enemy::DrawPosList()
{
	if (!posList_.empty())
	{
		VECTOR3 lineStartPos;
		VECTOR3 lineEndPos;

		for (int i = 0; i < posList_.size(); i++)
		{
			if (i == posList_.size() - 1)
			{
				DrawSphere3D(posList_[i], 50, 50, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
			}
			else
			{
				DrawSphere3D(posList_[i], 50, 50, GetColor(255, 255, 255), GetColor(255, 0, 0), TRUE);
			}
		}

		for (int i = 1; i < posList_.size(); i++)
		{
			lineStartPos = posList_[i - 1];
			lineEndPos = posList_[i];

			DrawLine3D(lineStartPos, lineEndPos, GetColor(0, 0, 0));
		}
	}
}
