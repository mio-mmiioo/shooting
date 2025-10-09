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
}

//bool StageObject::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
//{
//	MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hitModel_, -1, pos1, pos2);
//	if (ret.HitFlag == false)
//		return false;
//	if (hit != nullptr) {
//		*hit = ret.HitPosition;
//	}
//	return true;
//}
