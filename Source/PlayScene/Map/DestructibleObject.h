#pragma once
#include "../CanShoot.h"

class DestructibleObject : public CanShoot {
public:
	DestructibleObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale, const int& hp);
	~DestructibleObject();
	void Update();

private:

};