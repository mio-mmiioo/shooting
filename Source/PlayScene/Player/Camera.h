#pragma once
#include "../../../Library/Object3D.h"

class Camera : public Object3D {
public:
	Camera();
	~Camera();
	void Update() override;

	void SetPlayerPosition(const VECTOR& pos);
private:
	VECTOR lookPosition_;
	int prevX, prevY;//前のマウスを取っておく

	enum CAM_STATE {
		FIRST, // 一人称視点
		THIRD, // 三人称視点
		FREE,  // 自由に動かせるカメラ
		FIX	   // 固定カメラ
	};

	CAM_STATE state_; // カメラの状態

	// 一人称で使用する変数
	Transform firstTransform_;
	Transform thirdTransform_;
	Transform freeTransform_;

	float freeDistance_; // カメラとターゲットの距離

	void SetFreeCamera(); // 自由に動かせるカメラを定位置にセットする

	void FirstCamera();		// 一人称カメラ
	void ThirdCamera();		// 三人称カメラ
	void FreeCamera();		// 自由に動かせるカメラ
	void FixCamera();		// 固定カメラ
};