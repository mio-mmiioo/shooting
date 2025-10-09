#pragma once
#include "../Actor.h"

class Camera;

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;
private:
	Camera* camera_;
	int mouseX_, mouseY_;
	VECTOR3 wPointerPos_;


};

