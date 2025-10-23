#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"

namespace GameMaster {
	Player* player;
}

void GameMaster::Init()
{
	Area::SetStage(); // 最初のステージをセット
}

void GameMaster::Update()
{
	SetPlayerPos(); // 条件を満たさなきゃセットされない

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
		Area::SetStage();
		player = FindGameObject<Player>();
		player->SetPosition(Area::GetNextPosition());
	}
}