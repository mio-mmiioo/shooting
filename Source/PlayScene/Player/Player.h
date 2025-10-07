#pragma once
#include "../Actor.h"

class Camera;

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
private:
	Camera* camera_;
};

