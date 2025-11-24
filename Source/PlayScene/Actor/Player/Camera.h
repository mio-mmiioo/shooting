#pragma once
#include "../../../../Library/Object3D.h"

enum CAM_STATE {
	FIRST,		// 一人称視点
	FIRST_FREE, // 一人称で注視点を動かせるカメラ
	THIRD,		// 三人称視点
	FIX			// 固定カメラ
};

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Update() override;

	void SetPlayerPosition(const Transform& transform);
	
	CAM_STATE GetCameraState() { return state_; };

private:
	Transform look_;
	int prevX, prevY;//前のマウスを取っておく

	CAM_STATE state_; // カメラの状態

	VECTOR3 cameraPosition_; // 最終的にセットされる視点の位置
	VECTOR3 targetPosition_; // 最終的にセットされる注視点の位置

	VECTOR3 fixAddPosition_; // 固定カメラを移動させるときに使用する、加算された位置

	float freeDistance_; // カメラとターゲットの距離

	void FirstCamera();		// 一人称カメラ
	void FirstFreeCamera(); // 一人称で注視点を動かせるカメラ
	void ThirdCamera();		// 三人称カメラ
	void FixCamera();		// 固定カメラ
};