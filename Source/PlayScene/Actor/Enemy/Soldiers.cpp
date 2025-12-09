#include "Soldiers.h"
#include "../../../Sound.h"
#include "../../../Color.h"
#include "../../GameMaster.h"
#include "../../../ResultScene/Observer.h"
#include <assert.h>

namespace SOLDIERS
{
	VECTOR3 G = { 0.0f, 9.8f, 0.0f };
	VECTOR3 SIZE = { 50.0f, 200.0f, 50.0f }; // hitModelのサイズ

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 50.0f;

	VECTOR3 headshotPos = { 0.0f, 170.0f, 0.0f };
	float headshotR = 25.0f;
	int headshotBonus = 2;

	// 攻撃関連
	const float ATTACK_TIME = 6.0f;
	const int ATTACK_POWER = 2;
	const int ADD_POINT = 100;

	// 開発時のみ使用
	const float DIRECTION_LENGTH = 100.0f;
	const float POS_LIST_R = 50.0f;
	const int POS_LIST_DIV_NUM = 50;
}

Soldiers::Soldiers(const VECTOR3& position, float ang, int hp)
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
	gravity_ = SOLDIERS::G;
	distanceR_ = SOLDIERS::DISTANCE_R;
	moveSpeed_ = SOLDIERS::MOVE_SPEED;
	rotateSpeed_ = SOLDIERS::ROTATE_SPEED;
	state_ = E_STATE::WALK;

	// 攻撃関連
	attackTimer_ = SOLDIERS::ATTACK_TIME;
	attackPower_ = SOLDIERS::ATTACK_POWER;

	point_ = SOLDIERS::ADD_POINT;
	AddEnemy(this);
}

Soldiers::~Soldiers()
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

void Soldiers::Update()
{
	if (isAlive_ == false)
	{
		PlaySoundMem(Sound::se["BreakSOLDIERS"], DX_PLAYTYPE_BACK, TRUE);
		Observer::EnemyKilled();
		Observer::AddPoint(point_);
		DestroyMe();
	}

	switch (state_) // ステートベースAI
	{
	case E_STATE::WALK:
		UpdateWalk();
		break;
	case E_STATE::STAY:
		UpdateStay();
		break;
	case E_STATE::SEARCH:
		UpdateSearch();
		break;
	case E_STATE::ATTACK:
		UpdateAttack();
		break;
	}

	DevelopmentInput(); // 手動

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	// 位置情報の修正
	GameMaster::CheckSetPosition(transform_, time_, gravity_, distanceR_);

	// 位置情報の更新　これがないと、当たり判定の場所が更新されない
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Soldiers::Draw()
{
	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	DrawLine3D(transform_.position_ + LOOK_HEIGHT,
		transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * SOLDIERS::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), Color::WHITE);

	VECTOR3 checkPos1 = VECTOR3(-SOLDIERS::SIZE.x / 2, 0, -SOLDIERS::SIZE.z / 2) + transform_.position_;
	VECTOR3 checkPos2 = VECTOR3(SOLDIERS::SIZE.x / 2, SOLDIERS::SIZE.y, SOLDIERS::SIZE.z / 2) + transform_.position_;
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

void Soldiers::UpdateSearch()
{
}
