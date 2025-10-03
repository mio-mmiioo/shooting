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


