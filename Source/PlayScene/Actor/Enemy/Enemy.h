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

	void SetToGo(VECTOR3 goPosition) { goPosition_ = goPosition; }
	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	
	bool isAttacked_;

private:

	Player* player_;
	VECTOR3 goPosition_;
	bool isArrive_;

};