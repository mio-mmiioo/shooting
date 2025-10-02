#include "Camera.h"

namespace {
	static float distance = 500.0f; // キャラからの距離
	static float lookHeight = 200.0f; // 注視点の高さ
	const float CAMERA_SPEED = 0.005f; // カメラの回転速度
};

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform_.rotation_.y = 20.0f * DegToRad;
}

Camera::~Camera()
{
}

void Camera::Update()
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
	SetCameraPositionAndTarget_UpVecY(lookPosition + VECTOR(camPos) + VECTOR(playerHeadPos), lookPosition + VECTOR(VECTOR3(0, 200.0f, 0)));

	prevX = mouseX;
	prevY = mouseY;
}

void Camera::SetPlayerPosition(const VECTOR& pos)
{
	lookPosition = pos;
}
