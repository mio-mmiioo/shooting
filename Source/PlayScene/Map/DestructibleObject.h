#pragma once
#include "../../../Library/Object3D.h"

class DestructibleObject : public Object3D {
public:
	DestructibleObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale);
	~DestructibleObject();

private:

};