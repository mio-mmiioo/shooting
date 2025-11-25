#include "DestructibleObject.h"
#include <assert.h>

DestructibleObject::DestructibleObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale, const int& hp)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + fileName + ".mv1").c_str());
	hitModel_ = MV1LoadModel((folder + fileName + "_c.mv1").c_str());

	transform_.position_ = position;
	transform_.rotation_ = rotation;
	transform_.scale_ = scale;
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());

	MV1SetupCollInfo(hitModel_);
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);

	hp_ = hp;
}

DestructibleObject::~DestructibleObject()
{
	if (hModel_ > 0) {
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0) {
		MV1DeleteModel(hitModel_);
		hitModel_ = -1;
	}
}

void DestructibleObject::Update()
{
	if (hp_ <= 0)
	{
		DestroyMe();
	}
}
