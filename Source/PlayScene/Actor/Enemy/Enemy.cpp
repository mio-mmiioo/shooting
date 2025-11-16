#include "Enemy.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../../../../Library/Input.h"
#include "../../../ResultScene/Observer.h"
#include "../../GameMaster.h"
#include "../../../Sound.h"

namespace ENEMY
{
	VECTOR3 G = { 0, 9.8, 0 };
	VECTOR3 size = { 50, 200, 50 }; // hitModelのサイズ

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 100.0f;

	VECTOR3 headshotPos = { 0, 170, 0 };
	float headshotR = 25;
	int headshotBonus = 2;

	// 攻撃関連
	const float ATTACK_TIME = 6.0f;
}

Enemy::Enemy(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hp_ = hp;

	isAlive_ = true;
	isAttacked_ = false;

	goPosition_ = VECTOR3(0, 0, 0);
	isArrive_ = true;

	/*hModel_ = MV1LoadModel("data/model/enemy01.mv1");
	assert(hModel_ > 0);*/

	hitModel_ = MV1LoadModel("data/model/enemy01_c.mv1");
	assert(hitModel_ > 0);

	const std::string folder = "data/model/Enemy/";
	// キャラモデルを読む
	hModel_ = MV1LoadModel((folder + "BASE.mv1").c_str());
	assert(hModel_ > 0);
	// ルートノードをY軸回転する
	int root = MV1SearchFrame(hModel_, "root");
	MV1SetFrameUserLocalMatrix(hModel_, root, MGetRotY(DX_PI_F));

	animator_ = new Animator(hModel_);
	assert(animator_ != nullptr);
	animator_->AddFile(A_NEUTRAL,	folder + "Anim_Neutral.mv1" , true);
	animator_->AddFile(A_IDLE,		folder + "Anim_Idle.mv1"	, true);
	animator_->AddFile(A_WALK,		folder + "Anim_Walking.mv1" , true);
	animator_->AddFile(A_ATTACK,	folder + "Anim_Attack.mv1"  , false);

	transform_.MakeLocalMatrix();

	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());

	MV1SetupCollInfo(hitModel_);
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);

	stage_ = FindGameObject<Stage>();
	time_ = 0.0f;
	gravity_ = ENEMY::G;
	distanceR_ = ENEMY::DISTANCE_R;
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
	animator_->Play(ANIM_ID::A_WALK);
	animator_->Update();

	// 自動移動
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

	if (isArrive_ == false)
	{
		SetMove(goPosition_, 1.0f, 2.0f);

		// 壁があって、まっすぐに進めない、、、


		// 壁がなくてまっすぐに進める
		if (VSize(goPosition_ - transform_.position_) < ENEMY::DISTANCE_R + GameMaster::GetPlayerDistanceR())
		{
			isArrive_ = true;
			state_ = E_STATE::STAY;

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
	/*if (GameMaster::IsCanAttackPlayer(this) == true)
	{
		DrawCapsule3D(transform_.position_, transform_.position_ + VECTOR3(0, 180, 0), distanceR_, 8, GetColor(0, 0, 255), GetColor(255, 255, 255), FALSE);
	}
	else
	{
		DrawCapsule3D(transform_.position_, transform_.position_ + VECTOR3(0, 180, 0), distanceR_, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
	}*/

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

	//DrawFormatString(500, 10, GetColor(255, 255, 255), "AttackTimer:%f", attackTimer_);
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
			if (VSize(goPosition_ - transform_.position_) < ENEMY::DISTANCE_R + GameMaster::GetPlayerDistanceR())
			{
				isArrive_ = true;
				state_ = E_STATE::STAY;

			}
		}
	}
}

void Enemy::UpdateStay()
{
	animator_->Play(ANIM_ID::A_IDLE);
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

	if (isArrive_ == false)
	{
		state_ = E_STATE::WALK;
	}

}

void Enemy::UpdateAttack()
{
	animator_->Play(ANIM_ID::A_ATTACK);

	if (animator_->IsFinish())
	{
		state_ = E_STATE::STAY;
	}

}
