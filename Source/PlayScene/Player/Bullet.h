#pragma once
#include "../../../Library/Object3D.h"

class Bullet : public Object3D
{
public:
	Bullet();
	~Bullet();
	void Update() override;
	void Draw() override;

private:

};