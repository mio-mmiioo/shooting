#include "Player.h"

Player::Player(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;

	VECTOR3 cameraPosition;
	VECTOR3 cameraTarget;

	cameraPosition = VECTOR3(0, 300, -300) * MGetRotY(transform_.rotation_.y) + transform_.position_;
	cameraTarget = transform_.position_ + VECTOR3(0, 150, 0);//150足してるのは、カメラの中心をキャラの足元よりも高くするため
	SetCameraPositionAndTarget_UpVecY(cameraPosition, cameraTarget);

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

	VECTOR3 velocity;// 移動ベクトル
	if (CheckHitKey(KEY_INPUT_W))
	{
		velocity = VECTOR3(0, 0, 1) * 5.0f * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列
	}

	//velocity→進行方向
	transform_.position_ += velocity;

	VECTOR3 cameraPosition;
	VECTOR3 cameraTarget;

	cameraPosition = VECTOR3(0, 300, -300) * MGetRotY(transform_.rotation_.y) + transform_.position_;
	cameraTarget = transform_.position_ + VECTOR3(0, 150, 0);//150足してるのは、カメラの中心をキャラの足元よりも高くするため
	SetCameraPositionAndTarget_UpVecY(cameraPosition, cameraTarget);
}

void Player::Draw()
{
}
