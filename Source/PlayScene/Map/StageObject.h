#pragma once
#include "../../../Library/Object3D.h"

class StageObject : public Object3D {
public:
	StageObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale);
	~StageObject();

	bool CheckHit(VECTOR3 capsulePos1, VECTOR3 capsulePos2, float r);
private:

};