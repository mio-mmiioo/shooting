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
};