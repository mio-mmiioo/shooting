#include "StageObject.h"
#include <assert.h>

StageObject::StageObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder+fileName+".mv1").c_str());

	hitModel_ = MV1LoadModel((folder + fileName + "_c.mv1").c_str());

	transform_.position_ = position;
	transform_.rotation_ = rotation;
	transform_.scale_ = scale;
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());

	MV1SetupCollInfo(hitModel_);
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

StageObject::~StageObject()
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

bool StageObject::CheckHit(VECTOR3 capsulePos1, VECTOR3 capsulePos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM  ret = MV1CollCheck_Capsule(hitModel_, 0, capsulePos1, capsulePos2, r);

	// ƒ‚ƒfƒ‹‚ÆŠm‚©‚ß‚Ä‚¢‚é‚à‚Ì‚ª‚Ô‚Â‚©‚Á‚Ä‚¢‚éê‡
	if (ret.HitNum > 0)
	{
		return true;
	}

	return false;
}
