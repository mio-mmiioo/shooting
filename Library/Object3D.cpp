#include "Object3D.h"

Object3D::Object3D()
	:hModel_(-1), parent_(nullptr)
{
}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	if (hModel_ > 0)
	{
		const MATRIX& m = transform_.MakeLocalMatrix();
		if (parent_ != nullptr)
		{
			MV1SetMatrix(hModel_, m * parent_->GetTransform().GetLocalMatrix());
		}
		else
		{
			MV1SetMatrix(hModel_, m);
		}
		MV1DrawModel(hModel_);
	}
}

bool Object3D::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit) const
{
	MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hitModel_, -1, pos1, pos2);
	if (ret.HitFlag == false)
	{
		return false;
	}
	if (hit != nullptr) 
	{
		*hit = ret.HitPosition;
	}
	return true;
}



