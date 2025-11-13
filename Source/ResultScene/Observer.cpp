#include "Observer.h"

namespace Observer {
	int enemyKilledCounter = 0;
}

void Observer::Input()
{
}

void Observer::EnemyKilled()
{
	enemyKilledCounter += 1;
}

int Observer::GetEnemyKillled()
{
	return enemyKilledCounter;
}


