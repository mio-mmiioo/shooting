#pragma once
//#include "../Actor/Actor.h"
#include "../Destructible.h"

class DestructibleObject : public Destructible {
public:
	DestructibleObject(const std::string& fileName, const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale, const int& hp);
	~DestructibleObject();
	void Update() override;

private:

};