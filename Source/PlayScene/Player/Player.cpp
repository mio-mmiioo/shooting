#include "Player.h"
#include "Camera.h"
#include <assert.h>

Player::Player(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hModel_ = MV1LoadModel("data/model/player02.mv1");
	assert(hModel_ > 0);

	camera_ = FindGameObject<Camera>();
}

Player::~Player()
{
}

void Player::Update()
{
	//回転
	{
		if (CheckHitKey(KEY_INPUT_D))
		{
			transform_.rotation_.y += 3 * DegToRad;
		}
		if (CheckHitKey(KEY_INPUT_A))
		{
			transform_.rotation_.y -= 3 * DegToRad;
		}
	}

	//移動

	VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
	velocity = VECTOR3(0, 0, 1) * 5.0f * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

	if (CheckHitKey(KEY_INPUT_W))
	{
		transform_.position_ += velocity;
	}
	else if (CheckHitKey(KEY_INPUT_S))
	{
		transform_.position_ -= velocity;
	}


	camera_->SetPlayerPosition(transform_); // プレイヤーの情報をカメラにセット
}

void Player::Draw()
{
	Object3D::Draw();

	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = { 0, 180, 0 };
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * 100 * MGetRotY(transform_.rotation_.y), GetColor(255, 255, 255));
}
