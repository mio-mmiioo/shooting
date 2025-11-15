#pragma once

namespace Observer {
	void Init();
	void Release();

	void AddPoint(int addPoint);
	void EnemyKilled();
	int GetEnemyKillled();
	int GetPoint();
}
