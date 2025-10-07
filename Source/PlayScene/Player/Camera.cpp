#include "Camera.h"
#include "../../../Library/Input.h"

namespace {
	static float DISTANCE = 500.0f; // キャラからの距離
	static float LOOK_HIEGHT = 200.0f; // 注視点の高さ
	const float CAMERA_SPEED = 0.005f; // カメラの回転速度
};

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform_.rotation_.y = 20.0f * DegToRad;
	lookPosition_ = {0, 150, 0};
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));
	state_ = THIRD;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (Input::IsKeyDown(KEY_INPUT_0))
	{
		state_ = CAM_STATE::FIRST;
	}
	else if (Input::IsKeyDown(KEY_INPUT_1))
	{
		state_ = CAM_STATE::THIRD;
	}
	else if (Input::IsKeyDown(KEY_INPUT_2))
	{
		state_ = CAM_STATE::FREE;
	}
	else if (Input::IsKeyDown(KEY_INPUT_3))
	{
		state_ = CAM_STATE::FIX;
	}

	switch (state_)
	{
	case CAM_STATE::FIRST:
		FirstCamera();
		break;
	case CAM_STATE::THIRD:
		ThirdCamera();
		break;
	case CAM_STATE::FREE:
		FreeCamera();
		break;
	case CAM_STATE::FIX:
		FixCamera();
		break;
	}
}

void Camera::SetPlayerPosition(const VECTOR& pos)
{
	lookPosition_ = pos;
}

void Camera::FirstCamera()
{
}

void Camera::ThirdCamera()
{
	int mouseX, mouseY;

	GetMousePoint(&mouseX, &mouseY);
	int moveX = mouseX - prevX;
	int moveY = mouseY - prevY;

	VECTOR3& rot = transform_.rotation_;

	rot.y += CAMERA_SPEED * moveX;
	rot.x -= CAMERA_SPEED * moveY;

	if (rot.x >= 80 * DegToRad)
	{
		rot.x = 80 * DegToRad;
	}
	if (rot.x < -25 * DegToRad)
	{
		rot.x = -25 * DegToRad;
	}

	VECTOR3 playerHeadPos = VECTOR3(0, 150.0f, 0);
	VECTOR3 camPos = VECTOR3(0, 0, -500.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);

	//ここの書き方もう少しよくかけそう
	SetCameraPositionAndTarget_UpVecY(lookPosition_ + VECTOR(camPos) + VECTOR(playerHeadPos), lookPosition_ + VECTOR(VECTOR3(0, 200.0f, 0)));

	prevX = mouseX;
	prevY = mouseY;
}

void Camera::FreeCamera()
{
	// 移動
	float moveSpeed = 3.0f;
	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		freeTransform_.position_.y += moveSpeed;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		freeTransform_.position_.y -= moveSpeed;
	}

	// カメラの回転コメント解除するとカメラも回転しちゃう→使う場合は使うボタンを変える
	float RotSpeed = 3.0f; //回転の速さ(度）
	if (Input::IsKeepKeyDown(KEY_INPUT_R))
	{
		freeTransform_.rotation_.y -= RotSpeed * DX_PI_F / 180.0f;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_L))
	{
		freeTransform_.rotation_.y += RotSpeed * DX_PI_F / 180.0f;
	}

	// xy, yzはあとで考える

	// このwhile は if でもいいけど、while が必要なゲームもあるから、、今回はカメラ自分で動かす予定だから考えなくてもよい
	while (freeTransform_.rotation_.y >= DX_PI_F) {
		freeTransform_.rotation_.y -= 2.0f * DX_PI_F;
	}
	while (freeTransform_.rotation_.y < -DX_PI_F) {
		freeTransform_.rotation_.y += 2.0f * DX_PI_F;
	}

	// 上で考えた角度をもとに長さをセットする
	VECTOR tar;
	tar.x = freeTransform_.position_.x + 100 * cos(freeTransform_.rotation_.y);
	tar.z = freeTransform_.position_.z + 100 * sin(freeTransform_.rotation_.y);

	SetCameraPositionAndTarget_UpVecY(freeTransform_.position_, tar);//カメラの位置をセット
}

void Camera::FixCamera()
{
	VECTOR pos = { 0, 0, -300 };
	VECTOR tar = { 0, 0, 0 };
	SetCameraPositionAndTarget_UpVecY(pos, tar);//カメラの位置をセット
}
