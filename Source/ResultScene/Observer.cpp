#include "Observer.h"

namespace Observer {
	int enemyKilledCounter = 0;
	int point = 0;
}

void Observer::Init()
{
}

void Observer::Release()
{
	enemyKilledCounter = 0;
	point = 0;
}

void Observer::AddPoint(int addPoint)
{
	point += addPoint;
}

void Observer::EnemyKilled()
{
	enemyKilledCounter += 1;
}

int Observer::GetEnemyKillled()
{
	return enemyKilledCounter;
}

int Observer::GetPoint()
{
	return point;
}


