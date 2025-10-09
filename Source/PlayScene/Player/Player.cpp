#include "Player.h"
#include "Camera.h"
#include <assert.h>
#include "../Map/Stage.h"
#include "../../../Library/Input.h"

namespace PLAYER
{
	VECTOR3 G = { 0, 9.8, 0 };
}

Player::Player(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hModel_ = MV1LoadModel("data/model/player02.mv1");
	assert(hModel_ > 0);

	camera_ = FindGameObject<Camera>();
	stage_ = FindGameObject<Stage>();
	time_ = 0;
}

Player::~Player()
{
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
}

void Player::Update()
{
	GetMousePoint(&mouseX_, &mouseY_);

	VECTOR ScreenPos = { (float)mouseX_, (float)mouseY_, 1.0f };
	wPointerPos_ = ConvScreenPosToWorldPos(ScreenPos);

	//回転
	{
		if (Input::IsKeepKeyDown(KEY_INPUT_D))
		{
			transform_.rotation_.y += 3 * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_A))
		{
			transform_.rotation_.y -= 3 * DegToRad;
		}
	}

	//移動

	VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
	velocity = VECTOR3(0, 0, 1) * 5.0f * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

	if (Input::IsKeepKeyDown(KEY_INPUT_W))
	{
		transform_.position_ += velocity;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_S))
	{
		transform_.position_ -= velocity;
	}

	VECTOR3 hit;
	VECTOR3 startPos = transform_.position_ + VECTOR3(0, 180, 0);
	DrawLine3D(startPos, wPointerPos_, GetColor(255, 255, 255));

	if (stage_->CollideLine(startPos, wPointerPos_, &hit))
	{
		DrawSphere3D(hit, 20, 20, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	}

	stage_->SetOnGround(transform_.position_, time_, PLAYER::G); // ステージの位置を確認し、空中に浮いていないか確認する
	camera_->SetPlayerPosition(transform_); // プレイヤーの情報をカメラにセット
}

void Player::Draw()
{
	Object3D::Draw();

	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = { 0, 180, 0 };
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * 100 * MGetRotY(transform_.rotation_.y), GetColor(255, 255, 255));
}
