#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"

namespace GameMaster {
	Player* player;
}

void GameMaster::Update()
{
	SetPlayerPos(); // ğŒ‚ğ–‚½‚³‚È‚«‚áƒZƒbƒg‚³‚ê‚È‚¢

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
		player->SetPosition(Area::GetNextPosition());
	}
}