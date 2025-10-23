#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"

namespace GameMaster {
	Player* player;
}

void GameMaster::Update()
{
	SetPlayerPos();

}

bool GameMaster::IsChangeArea()
{
	std::list<Enemy*> enemy = FindGameObjects<Enemy>();
	if (enemy.empty())
	{
		return true;
	}
	return false;
}

void GameMaster::SetPlayerPos()
{
	if (IsChangeArea() == true)
	{
		player = FindGameObject<Player>();
		VECTOR3 newPos = { 0, 0, 0 };
		player->SetPosition(newPos);
	}
}
