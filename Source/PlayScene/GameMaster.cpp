#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"

namespace GameMaster {
	Player* player = nullptr;
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
		Area::SetNextPosition();
		Area::SetStage(); // Area::SetNextPosition()の後にかく
	}

	player = FindGameObject<Player>();

	if (player != nullptr)
	{
		if (VSize(player->GetTransform().position_ - Area::GetCurrentPosition()) > 50.0f)
		{
			player->SetToGo(Area::GetCurrentPosition());
			player->SetIsArrive(false);
		}
	}
}
