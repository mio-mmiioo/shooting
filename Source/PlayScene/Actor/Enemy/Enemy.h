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

	//bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const override;

	bool isAttacked_;


private:
	Player* player_;
};