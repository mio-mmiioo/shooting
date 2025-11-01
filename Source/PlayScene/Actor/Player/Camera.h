#pragma once
#include "../../../../Library/Object3D.h"

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Update() override;

	void SetPlayerPosition(const Transform& transform);
	VECTOR3 GetTargetPosition() { return targetPosition_; }
private:
	Transform look_;
	int prevX, prevY;//前のマウスを取っておく

	enum CAM_STATE {
		FIRST,		// 一人称視点
		FIRST_FREE, // 一人称で注視点を動かせるカメラ
		THIRD,		// 三人称視点
		FREE,		// 自由に動かせるカメラ
		FIX			// 固定カメラ
	};

	CAM_STATE state_; // カメラの状態

	VECTOR3 cameraPosition_; // 最終的にセットされる視点の位置
	VECTOR3 targetPosition_; // 最終的にセットされる注視点の位置

	Transform firstTransform_;
	Transform thirdTransform_;
	Transform freeTransform_;

	float freeDistance_; // カメラとターゲットの距離

	void SetFreeCamera(); // 自由に動かせるカメラを定位置にセットする

	void FirstCamera();		// 一人称カメラ
	void FirstFreeCamera(); // 一人称で注視点を動かせるカメラ
	void ThirdCamera();		// 三人称カメラ
	void FreeCamera();		// 自由に動かせるカメラ
	void FixCamera();		// 固定カメラ
};