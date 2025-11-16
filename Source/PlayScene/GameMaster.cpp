#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"
#include "../Sound.h"

namespace GameMaster {
	void SetPlayerPos();
	void SetEnemyPos();

	Player* player = nullptr;
	Stage* stage = nullptr;
	std::list<Enemy*> enemy;
	std::list<Enemy*> hitEnemy;
}

void GameMaster::Init()
{
	Area::SetStage(); // 最初のステージをセット
	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();
	stage = FindGameObject<Stage>();
}

void GameMaster::Update()
{
	// 敵に銃弾を当てる処理
	{
		VECTOR3 ret = { 10000, 10000, 10000 };
		float distance = ((VECTOR3)(player->GetTransform().position_ - ret)).Size();
		Enemy* attackedEnemy = nullptr;
		for (Enemy* enemy : hitEnemy)
		{
			VECTOR3 ePos = enemy->GetTransform().position_;
			float d = ((VECTOR3)(player->GetTransform().position_ - ePos)).Size();

			if (player->Attack() > 0) // 攻撃が0より大きい→攻撃してる場合
			{
				if (distance > d)
				{
					distance = d;
					attackedEnemy = enemy;
				}
			}
		}

		if (player->Attack() > 0)
		{
			if (attackedEnemy != nullptr) // ポインターが敵にあっていない場合、attackedEnemyがnullptrになっている
			{
				PlaySoundMem(Sound::se["AttackEnemy"], DX_PLAYTYPE_BACK, TRUE);
				attackedEnemy->addHp(-player->Attack());
			}
		}

		hitEnemy.clear();
	}

	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();

	SetPlayerPos(); // 条件を満たさなきゃセットされない
	SetEnemyPos();
}

void GameMaster::Release()
{
	Area::Release();
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
			if (VSize(e->GetTransform().position_ - player->GetTransform().position_) > 200.0f)
			{
				e->SetToGo(player->GetTransform().position_);
				e->SetIsArrive(false);
			}
		}
	}
}

bool GameMaster::IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos)
{
	VECTOR3 hit;
	for (Enemy* enemy : enemy) // 銃弾が当たる場所にいる敵のリストを作成する
	{
		if (enemy->Object3D::CollideLine(startPos, endPos, &hit))
		{
			hitEnemy.push_back(enemy);
		}
	}

	if (hitEnemy.size() > 0) // 1以上なら当たる
	{
		return true;
	}
	return false;
}

void GameMaster::CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR)
{
	stage->SetOnGround(transform.position_, time, gravity); // ステージの位置を確認し、空中に浮いていないか確認する 浮いていたら重力をかける
	VECTOR3 front = transform.position_ + VECTOR3(0, 0, 1) *  100 * MGetRotY(transform.rotation_.y);
	VECTOR3 back  = transform.position_ + VECTOR3(0, 0, 1) * -100 * MGetRotY(transform.rotation_.y);
	stage->CheckPush(transform.position_, front, distanceR); // ステージへのめり込みを確認する(前方)
	stage->CheckPush(transform.position_, back, distanceR);  // ステージへのめり込みを確認する(後方)

	// Actorどうしの当たり判定
}
