#include "Camera.h"
#include "../../../../Library/Input.h"

namespace {
	static float DISTANCE = 500.0f; // キャラからの距離
	static float LOOK_HIEGHT = 180.0f; // 視点の高さ
	const float CAMERA_SPEED = 0.01f; // カメラの回転速度
};

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform_.rotation_.y = 20.0f * DegToRad;
	freeDistance_ = 1.0f;

	cameraPosition_ = VECTOR3(50, 100, -300);
	targetPosition_ = VECTOR3(0, 0, 0);
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);

	state_ = CAM_STATE::FIX;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	SetCameraNearFar(50.0f, 2000.0f); // ここにいらないかも
	// カメラのセットを切り替える
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
			SetFreeCamera();
		}
		else if (Input::IsKeyDown(KEY_INPUT_3))
		{
			state_ = CAM_STATE::FIX;
		}
		else if (Input::IsKeyDown(KEY_INPUT_4))
		{
			state_ = CAM_STATE::FIRST_FREE;
		}

	}

	// カメラ状態によって、セットする位置を調整
	switch (state_)
	{
	case CAM_STATE::FIRST:
		FirstCamera();
		break;
	case CAM_STATE::FIRST_FREE:
		FirstFreeCamera();
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

	// カメラの位置をセット
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
}

void Camera::SetPlayerPosition(const Transform& transform)
{
	look_ = transform;
}

void Camera::SetFreeCamera()
{
	freeTransform_.position_ = VECTOR3(0, 0, -300);
}

void Camera::FirstCamera()
{
	cameraPosition_ = look_.position_ + VECTOR3(0, LOOK_HIEGHT, 0); // 目線の高さに合わせてる
	targetPosition_ = look_.position_ + VECTOR3(0, LOOK_HIEGHT, 0) + VECTOR3(0, 0, 1) * 1000 * MGetRotY(look_.rotation_.y);
}

void Camera::FirstFreeCamera()
{
	cameraPosition_ = look_.position_ + VECTOR3(0, LOOK_HIEGHT, 0); // 目線の高さに合わせてる

	// 注視点を動かすプログラム
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

	VECTOR3 playerHeadPos = VECTOR3(0, 180.0f, 0);
	VECTOR3 camPos = VECTOR3(0, 0, -500.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);

	targetPosition_ = look_.position_ + camPos + playerHeadPos;

	prevX = mouseX;
	prevY = mouseY;

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

	VECTOR3 playerHeadPos = VECTOR3(0, 180.0f, 0);
	VECTOR3 camPos = VECTOR3(0, 0, -500.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);

	cameraPosition_ = look_.position_ + camPos + playerHeadPos;
	targetPosition_ = look_.position_ + VECTOR3(0, 200.0f, 0); // プレイヤーの頭より少し上を見るように

	prevX = mouseX;
	prevY = mouseY;
}

void Camera::FreeCamera()
{
	//// カメラの回転コメント解除するとカメラも回転しちゃう→使う場合は使うボタンを変える
	//float RotSpeed = 3.0f; //回転の速さ(度）
	//if (Input::IsKeepKeyDown(KEY_INPUT_R))
	//{
	//	freeTransform_.rotation_.y -= RotSpeed * DX_PI_F / 180.0f;
	//}
	//else if (Input::IsKeepKeyDown(KEY_INPUT_L))
	//{
	//	freeTransform_.rotation_.y += RotSpeed * DX_PI_F / 180.0f;
	//}

	//// 移動
	//{
	//	float moveSpeed = 3.0f;
	//	
	//	// 上下
	//	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	//	{
	//		freeTransform_.position_.y += moveSpeed;
	//	}
	//	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	//	{
	//		freeTransform_.position_.y -= moveSpeed;
	//	}

	//	VECTOR3 velocity;// 移動ベクトル
	//	if (CheckHitKey(KEY_INPUT_G))
	//	{
	//		velocity = VECTOR3(0, 0, 1) * moveSpeed * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列
	//	}

	//	//velocity→進行方向
	//	freeTransform_.position_ += velocity;
	//}

	//// このwhile は if でもいいけど、while が必要なゲームもあるから、、今回はカメラ自分で動かす予定だから考えなくてもよい
	//while (freeTransform_.rotation_.y >= DX_PI_F) {
	//	freeTransform_.rotation_.y -= 2.0f * DX_PI_F;
	//}
	//while (freeTransform_.rotation_.y < -DX_PI_F) {
	//	freeTransform_.rotation_.y += 2.0f * DX_PI_F;
	//}

	//// 上で考えた角度をもとに長さをセットする 多分ここがうまくいってない
	//VECTOR3 tar;
	//tar.x = freeTransform_.position_.x + freeDistance_ * cos(freeTransform_.rotation_.y);
	//tar.z = freeTransform_.position_.z + freeDistance_ * sin(freeTransform_.rotation_.y);

	//// カメラの位置を代入
	//cameraPosition_ = freeTransform_.position_;
	//targetPosition_ = tar;
}

void Camera::FixCamera()
{
	cameraPosition_ = VECTOR3(0, 0, -1000);
	targetPosition_ = VECTOR3(0, 0, 0);
}
