#pragma once
#include "Enemy.h"

class Soldiers : public Enemy
{
public:
	Soldiers(const VECTOR3& position, float ang, int hp);
	~Soldiers() override;
	void Update() override;
	void Draw() override;
private:
	void UpdateSearch();

	enum E_STATE {
		STAY,
		WALK,
		SEARCH,
		ATTACK,
		MAX_STATE
	};

	E_STATE state_;
};
