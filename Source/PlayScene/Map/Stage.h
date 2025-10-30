#pragma once
#include "../../../Library/Object3D.h"

class SphereCollider;

class Stage : public Object3D {
public:
	Stage(int number = 0);
	~Stage();
	struct ColliderRet {
		VECTOR3 VPush; // 垂直方向の押し出し
		VECTOR3 HPush; // 水平方向の押し出し
	};

	bool CollideLine(const VECTOR3& pos1, const VECTOR3& pos2, VECTOR3* hit = nullptr) const; // 線でステージとオブジェクトの当たり判定
	void SetOnGround(VECTOR3& pos, float& time, VECTOR3 Gravity); // 線で当たり判定後、にposをステージに合わせてセットする
	void CheckPush(VECTOR3& pos1, VECTOR3 pos2, float minDistance);

private:
	void ReadMappingData(std::string filename);
};