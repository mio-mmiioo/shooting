#pragma once
#include "../Actor.h"

class Camera;
class Stage;

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;
private:
	Camera* camera_;
	Stage* stage_;
	float time_; // —Ž‰ºŽžŠÔ
};

