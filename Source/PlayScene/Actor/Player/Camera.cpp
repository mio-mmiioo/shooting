#include "Camera.h"
#include "../../../../Library/Input.h"

namespace {
	const float CAMERA_MOVE_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f; // カメラの回転速度
	const float CAMERA_NEAR = 50.0f; // カメラに映る手前の距離
	const float CAMERA_FAR = 15000.0f; // カメラに映る奥の距離

	// 一人称視点関連
	const float FIRST_DISTANCE = 1000.0f; // 視点と注視点の距離

	// 三人称視点関連
	const float MAX_ROTATE_X = 80.0f;
	const float MIN_ROTATE_X = -25.0f;
	const VECTOR3 THIRD_BASE_POSITION = { 0.0f, 0.0f, -500.0f };
	const VECTOR3 ADD_HEIGHT = LOOK_HEIGHT + VECTOR3(0.0f, 20.0f, 0.0f); // プレイヤーの頭より少し上を見る

	// 固定視点関連
	const VECTOR3 FIX_BASE_POSITION = { 0, 9000.0f, -1000.0f };
	const float ADD_DISTANCE = 100.0f;
};

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);

	fixAddPosition_ = VECTOR3(0.0f, 0.0f, 0.0f);
	FixCamera();
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);

	state_ = CAM_STATE::FIX;

	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
	wheelRot_ = 0;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// カメラのセットを切り替える
	{
		if (Input::IsKeyDown(KEY_INPUT_1))
		{
			state_ = CAM_STATE::THIRD;
		}
		else if (Input::IsKeyDown(KEY_INPUT_2))
		{
			state_ = CAM_STATE::FIRST;
		}
		else if (Input::IsKeyDown(KEY_INPUT_3))
		{
			state_ = CAM_STATE::FIX;
		}

	}

	// カメラ状態によって、セットする位置を調整
	switch (state_)
	{
	case CAM_STATE::FIRST:
		FirstCamera();
		break;
	case CAM_STATE::THIRD:
		ThirdCamera();
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

void Camera::FirstCamera()
{
	// 自動で動かす予定
	cameraPosition_ = look_.position_ + LOOK_HEIGHT; // 目線の高さに合わせてる
	targetPosition_ = look_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * FIRST_DISTANCE * MGetRotY(look_.rotation_.y);
}

void Camera::ThirdCamera()
{
	int mouseX, mouseY;

	GetMousePoint(&mouseX, &mouseY);
	int moveX = mouseX - prevX;
	int moveY = mouseY - prevY;

	VECTOR3& rot = transform_.rotation_;

	rot.y += CAMERA_ROTATE_SPEED * moveX;
	rot.x -= CAMERA_ROTATE_SPEED * moveY;

	if (rot.x >= MAX_ROTATE_X * DegToRad)
	{
		rot.x = MAX_ROTATE_X * DegToRad;
	}
	if (rot.x < MIN_ROTATE_X * DegToRad)
	{
		rot.x = MIN_ROTATE_X * DegToRad;
	}

	VECTOR3 camPos = THIRD_BASE_POSITION * MGetRotX(rot.x) * MGetRotY(rot.y);

	cameraPosition_ = look_.position_ + camPos + LOOK_HEIGHT;
	targetPosition_ = look_.position_ + ADD_HEIGHT;

	prevX = mouseX;
	prevY = mouseY;
}

void Camera::FixCamera()
{
	wheelRot_ += GetMouseWheelRotVol();
	
	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		fixAddPosition_.z += CAMERA_MOVE_SPEED;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		fixAddPosition_.z -= CAMERA_MOVE_SPEED;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		fixAddPosition_.x += CAMERA_MOVE_SPEED;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		fixAddPosition_.x -= CAMERA_MOVE_SPEED;
	}

	cameraPosition_ = FIX_BASE_POSITION + VECTOR3(0.0f, -(float)(wheelRot_ * ADD_DISTANCE), 0.0f) + fixAddPosition_;
	targetPosition_ = fixAddPosition_;
}
