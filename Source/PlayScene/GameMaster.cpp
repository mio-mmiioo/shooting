#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"
#include "Map/DestructibleObject.h"
#include "Map/WayInfo.h"
#include "../Sound.h"
#include "../../Library/Input.h"

#include <algorithm>

namespace GameMaster {
	void SetPlayerPos();
	void SetEnemyPos();

	// 銃弾の当たり判定関連
	bool IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos); // 銃弾が敵に当たるか あたるならtrue 今プレイヤーで呼び出してるのは消す
	bool IsBulletHitStageObject(VECTOR3& pos1, const VECTOR3& pos2);
	bool IsBulletHitDestructibleObject(VECTOR3& pos1, const VECTOR3& pos2);

	// 定数
	const float CHECK_FRONT_LENGTH = 100.0f;
	const float CHECK_BACK_LENGTH = 100.0f;

	Player* player = nullptr;
	Stage* stage = nullptr;
	std::list<Enemy*> enemy;
	std::list<Enemy*> hitEnemy;

	std::list<DestructibleObject*> destructibleObject;
	std::list<DestructibleObject*> hitDestructibleObject;

	VECTOR3 enemyHit;
	VECTOR3 stageHit;
	VECTOR3 destructibleHit;
}

void GameMaster::Init()
{
	WayInfo::Init();
	//Area::SetStage(); // 最初のステージをセット
	new Stage(2); // 建物邪魔な場合のステージ
	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();
	stage = FindGameObject<Stage>();
	player->SetPosition(WayInfo::SetVertexPosition(player->GetTransform().position_, 1));
	player->SetPrevVretexPosition(player->GetTransform().position_);

	for (auto e : enemy)
	{
		e->SetPosition(WayInfo::SetVertexPosition(e->GetTransform().position_, 2));
	}
}

void GameMaster::Update()
{
	// 銃弾を当てる処理
	{
		if (hitEnemy.size() > 0)
		{
			float distance = ((VECTOR3)(player->GetTransform().position_ - stageHit)).Size();
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

		if (hitDestructibleObject.size() > 0)
		{
			float distance = ((VECTOR3)(player->GetTransform().position_ - stageHit)).Size();
			DestructibleObject* attackedDestructibleObject = nullptr;
			for (DestructibleObject* dObj : hitDestructibleObject)
			{
				VECTOR3 ePos = dObj->GetTransform().position_;
				float d = ((VECTOR3)(player->GetTransform().position_ - ePos)).Size();

				if (player->Attack() > 0) // 攻撃が0より大きい→攻撃してる場合
				{
					if (distance > d)
					{
						distance = d;
						attackedDestructibleObject = dObj;
					}
				}
			}

			if (player->Attack() > 0)
			{
				if (attackedDestructibleObject != nullptr)
				{
					PlaySoundMem(Sound::se["AttackEnemy"], DX_PLAYTYPE_BACK, TRUE);
					attackedDestructibleObject->addHp(-player->Attack());
				}
			}

			hitDestructibleObject.clear();
		}
	}

	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();
	destructibleObject = FindGameObjects<DestructibleObject>();

	
	SetPlayerPos(); // 条件を満たさなきゃセットされない
	SetEnemyPos();
	
	if (Input::IsKeyDown(KEY_INPUT_R) || Input::IsJoypadDown(XINPUT_BUTTON_Y)) {
		SceneManager::ChangeScene("RESULT");
	}
}

void GameMaster::Draw()
{
	WayInfo::WayDraw();
	//WayInfo::DrawVertex();
}

void GameMaster::Release()
{
	Area::Release();
}

void GameMaster::SetPlayerPos()
{
	if (enemy.empty()) // プレイヤーの移動条件を増やしていく予定
	{
		new Stage(1);
		//Area::SetNextPosition();
		//Area::SetStage(); // Area::SetNextPosition()の後にかく
	}
	if (player != nullptr)
	{
		// 頂点にいる
		if (WayInfo::IsVertexPosition(player->GetTransform().position_) == true)
		{
			if (player->GetArrive() == false)
			{
				for (auto e : enemy)
				{
					e->SetIsArrive(false);
				}
				player->SetPrevVretexPosition(player->GetTransform().position_);
				player->SetIsArrive(true);
			}
		}
		// 頂点にいない場合 ( 目的地についていない場合 )
		else
		{
			player->SetIsArrive(false);
		}
	}
}

void GameMaster::SetEnemyPos()
{
	VECTOR3 goPosition;
	VECTOR3 ePosition;
	VECTOR3 pPosition = player->GetPrevVertexPosition();
	for (auto e : enemy)
	{
		if (e->GetIsArrive() == false)
		{
			if (WayInfo::IsVertexPosition(e->GetTransform().position_))
			{
				goPosition = WayInfo::GetShortestWayPosition(e->GetTransform().position_, pPosition);
				e->SetGoPosition(goPosition);
			}
			
			ePosition = e->GetTransform().position_;
			if (VSize(ePosition - goPosition) < 50.0f)
			{
				if (VSize(ePosition - pPosition) > GetDistanceToPlayer(e->GetDistanceR()))
				{
					e->SetGoPosition(pPosition);
				}
				else
				{
					e->SetIsArrive(true);
				}
			}
		}
	}
}

void GameMaster::PlayerDeath()
{
	SceneManager::ChangeScene("RESULT");
}

bool GameMaster::IsBulletHit(VECTOR3 startPos, VECTOR3 endPos)
{
	bool ret = false;
	IsBulletHitStageObject(startPos, endPos);
	if (IsBulletHitEnemy(startPos, endPos) == true)
	{
		ret = true;
	}

	if (IsBulletHitDestructibleObject(startPos, endPos) == true)
	{
		ret = true;
	}
	
	// 一番プレイヤーに近いオブジェクトを探す
	{
		float enemyDistance = VSize(player->GetTransform().position_ - enemyHit);
		float stageDistance = VSize(player->GetTransform().position_ - stageHit);
		float destructibleDistance = VSize(player->GetTransform().position_ - destructibleHit);
		float minDistance = min(enemyDistance, destructibleDistance);
		if (minDistance == enemyDistance)
		{
			hitDestructibleObject.clear();
		}
		else if (minDistance == destructibleDistance)
		{
			hitEnemy.clear();
		}
		minDistance = min(minDistance, stageDistance);
		if (minDistance == stageDistance)
		{
			hitDestructibleObject.clear();
			hitEnemy.clear();
			ret = false;
		}
	}

	return ret;
}

bool GameMaster::IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos)
{
	for (Enemy* e : enemy) // 銃弾が当たる場所にいる敵のリストを作成する
	{
		if (e->Object3D::CollideLine(startPos, endPos, &enemyHit))
		{
			hitEnemy.push_back(e);
		}
	}

	if (hitEnemy.size() > 0) // 1以上なら当たる
	{
		return true;
	}
	return false;
}

bool GameMaster::IsBulletHitStageObject(VECTOR3& pos1, const VECTOR3& pos2)
{
	if (stage->CollideLineStageObject(pos1, pos2, &stageHit) == true)
	{
		return true;
	}
	stageHit = VECTOR3(10000.0f, 10000.0f, 10000.0f); // ヒットするものが見つからなかった場合 大きい値を代入する

	return false;
}

bool GameMaster::IsBulletHitDestructibleObject(VECTOR3& pos1, const VECTOR3& pos2)
{
	for (DestructibleObject* dObj : destructibleObject) // 銃弾が当たる場所にいる敵のリストを作成する
	{
		if (dObj->Object3D::CollideLine(pos1, pos2, &destructibleHit))
		{
			hitDestructibleObject.push_back(dObj);
		}
	}

	if (hitDestructibleObject.size() > 0) // 1以上なら当たる
	{
		return true;
	}
	return false;
}

// 現在地がめり込んでないか、地面に足がついているか
void GameMaster::CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR)
{
	stage->SetOnGround(transform.position_, time, gravity); // ステージの位置を確認し、空中に浮いていないか確認する 浮いていたら重力をかける
	VECTOR3 front = transform.position_ + VECTOR3(0, 0, 1) * CHECK_FRONT_LENGTH * MGetRotY(transform.rotation_.y);
	VECTOR3 back  = transform.position_ + VECTOR3(0, 0, 1) * -CHECK_BACK_LENGTH * MGetRotY(transform.rotation_.y);
	stage->CheckPush(transform.position_, front, distanceR); // ステージへのめり込みを確認する(前方)
	stage->CheckPush(transform.position_, back, distanceR);  // ステージへのめり込みを確認する(後方)

	// Actorどうしの当たり判定
}

bool GameMaster::IsCanAttackPlayer(Enemy* enemy)
{
	float min = (*enemy).GetDistanceR() + player->GetDistanceR(); // ぶつかる距離
	VECTOR3 aCap2 = (*enemy).GetTransform().position_ + LOOK_HEIGHT * (*enemy).GetTransform().GetRotationMatrix();
	VECTOR3 pCap2 = player->GetTransform().position_ + LOOK_HEIGHT * player->GetTransform().GetRotationMatrix();
	float checkDistance = Segment_Segment_MinLength(player->GetTransform().position_, pCap2, (*enemy).GetTransform().position_, aCap2); // 二つの線分の最近点間の距離を得る
	if (checkDistance < min) // ぶつかっている
	{
		return true;
	}
	return false;
}

void GameMaster::AttackPlayer(int atackPower)
{
	player->Attacked(atackPower);
}

float GameMaster::GetDistanceToPlayer(float distance)
{
	float ret = player->GetDistanceR() + distance;
	return ret;
}

