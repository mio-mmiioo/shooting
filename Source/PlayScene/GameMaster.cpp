#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"

namespace GameMaster {
	Player* player = nullptr;
	std::list<Enemy*> enemy;
}

void GameMaster::Init()
{
	Area::SetStage(); // 最初のステージをセット
}

void GameMaster::Update()
{
	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();

	SetPlayerPos(); // 条件を満たさなきゃセットされない
}

void GameMaster::SetPlayerPos()
{
	if (enemy.empty()) // プレイヤーの移動条件を増やしていく予定
	{
		Area::SetNextPosition();
		Area::SetStage(); // Area::SetNextPosition()の後にかく
	}

	if (player != nullptr)
	{
		if (VSize(player->GetTransform().position_ - Area::GetCurrentPosition()) > 50.0f)
		{
			player->SetToGo(Area::GetCurrentPosition());
			player->SetIsArrive(false);
		}
	}
}

void GameMaster::SetEnemyPos()
{
	if (!enemy.empty())
	{
		for (auto e : enemy)
		{
			e->SetToGo(player->GetTransform().position_);
			e->SetIsArrive(false);
		}
	}
}
