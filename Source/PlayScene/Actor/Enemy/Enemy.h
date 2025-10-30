#pragma once
#include "../Actor.h"

class Player;

class Enemy : public Actor
{
public:
	Enemy(const VECTOR3& position, float ang, int hp);
	~Enemy() override;
	void Update() override;
	void Draw() override;
	bool isAttacked_;

private:
	Player* player_;
};