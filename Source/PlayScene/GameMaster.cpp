#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"
#include "Map/DestructibleObject.h"
#include "Map/WayInfo.h"
#include "../Sound.h"
#include "../Color.h"
#include "../../Library/Input.h"
#include "Destructible.h"
#include <algorithm>

namespace GameMaster {
	void SetPlayerPos();
	void SetEnemyPos();

	bool IsBulletHitStageObject(VECTOR3& pos1, const VECTOR3& pos2); // 銃弾の当たり判定
	
	void SetIsDraw(); // 描画フラグのセット

	// 定数
	const float CHECK_FRONT_LENGTH = 100.0f;
	const float CHECK_BACK_LENGTH = 100.0f;

	Player* player = nullptr;
	Stage* stage = nullptr;
	std::list<Enemy*> enemy;
	std::list<Enemy*> hitEnemy;

	//std::list<DestructibleObject*> destructibleObject;
	//std::list<DestructibleObject*> hitDestructibleObject;

	VECTOR3 enemyHit;
	VECTOR3 stageHit;
	VECTOR3 destructibleHit;

	// 今回追加する部分
	Destructible* destructible = nullptr;

}

void GameMaster::Init()
{
	WayInfo::Init();
	//Area::SetStage(); // 最初のステージをセット
	new Stage(5); // 建物邪魔な場合のステージ
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
		VECTOR3 hit;
		if (player->Attack() > 0)
		{
			Destructible* attackedDestructibleObject = player->CheckHitDestructible(&hit);
			if (attackedDestructibleObject != nullptr)
			{
				PlaySoundMem(Sound::se["AttackEnemy"], DX_PLAYTYPE_BACK, TRUE);
				attackedDestructibleObject->addHp(-player->Attack());
			}
		}
	}

	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();
	
	SetPlayerPos(); // 条件を満たさなきゃセットされない
	SetEnemyPos();
	//SetIsDraw();

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
	VECTOR3 hit;
	IsBulletHitStageObject(startPos, endPos); // 破壊できないオブジェクトの当たり判定
	if (player->CollideLine(startPos, endPos, &hit) == true) // 破壊可能オブジェクトの当たり判定
	{
		ret = true;
	}
	
	// 一番プレイヤーに近いオブジェクトを探す
	float stageDistance = VSize(player->GetTransform().position_ - stageHit);
	float destructibleDistance = VSize(player->GetTransform().position_ - hit);
	if (destructibleDistance > stageDistance)
	{
		ret = false;
	}


	return ret;
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

void GameMaster::SetIsDraw()
{
	// 描画するか否かの判定をセットする 途中
	VECTOR3 base = player->GetTransform().position_;
	base = base + VECTOR3(0, 0, -100) * MGetRotY(player->GetTransform().rotation_.y);

	DrawSphere3D(base, 100.0f, 20, Color::WHITE, Color::WHITE, TRUE);

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

