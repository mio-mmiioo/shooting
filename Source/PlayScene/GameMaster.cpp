#include "GameMaster.h"
#include "../../Library/Object3D.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Map/Area.h"
#include "Map/Stage.h"
#include "Map/WayInfo.h"
#include "../Sound.h"

#include "../../Library/Input.h"

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
	WayInfo::Init();
	Area::SetStage(); // 最初のステージをセット
	//new Stage(6);
	player = FindGameObject<Player>();
	enemy = FindGameObjects<Enemy>();
	stage = FindGameObject<Stage>();
	player->SetPosition(WayInfo::SetVertexPosition(player->GetTransform().position_, 1));

	for (auto e : enemy)
	{
		e->SetPosition(WayInfo::SetVertexPosition(e->GetTransform().position_, 2));
	}
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
	if (WayInfo::IsVertexPosition(player->GetTransform().position_) == true)
	{
		SetEnemyPos();
	}

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
		Area::SetNextPosition();
		Area::SetStage(); // Area::SetNextPosition()の後にかく
	}
	if (player != nullptr)
	{
		// 頂点にいる
		if (WayInfo::IsVertexPosition(player->GetTransform().position_) == true)
		{
			if (player->GetArrive() == false)
			{
				WayInfo::SetVertexPosition(player->GetTransform().position_, WayInfo::CheckVertexNum(player->GetTransform().position_));
				for (auto e : enemy)
				{
					e->SetPosList(WayInfo::GetShortestWayPosition(e->GetTransform().position_, player->GetTransform().position_));
				}
				player->SetIsArrive(true);
			}
		}
		// 頂点にいない場合 ( 目的地についていない場合 )
		else
		{
			player->SetIsArrive(false);
		}

		

		//if (VSize(player->GetTransform().position_ - Area::GetCurrentPosition()) > 50.0f)
		//{
		//	player->SetToGo(Area::GetCurrentPosition());
		//	player->SetIsArrive(false);
		//}
	}
}

void GameMaster::SetEnemyPos()
{
	if (!enemy.empty())
	{
		for (auto e : enemy)
		{
			// wayListのサイズが1以上なら
			// 今の場所がwayList[wayList.size() -1]のポジションと同じ場合
			// goPositionを切り替える
			// 移動する

			// playerが位置更新したらエネミーは最短経路を取得する
			// 最短経路を取得した時は、endPosを設定する
			
			int wayListSize = e->GetWayList().size() - 1;

			//if (!((int)e->GetWayList()[wayListSize].x / WayInfo::BOX_SIZE == (int)e->GetTransform().position_.x / WayInfo::BOX_SIZE && (int)e->GetWayList()[wayListSize].z / WayInfo::BOX_SIZE == (int)e->GetTransform().position_.z / WayInfo::BOX_SIZE))
			//{

			//}

			//if (VSize(e->GetTransform().position_ - player->GetTransform().position_) > e->GetDistanceR() + player->GetDistanceR())
			//{
			//	//e->SetToGo(player->GetTransform().position_);
			//	//e->SetIsArrive(false);
			//}
		}
	}
}

void GameMaster::PlayerDeath()
{
	SceneManager::ChangeScene("RESULT");
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

// 現在地がめり込んでないか、地面に足がついているか
void GameMaster::CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR)
{
	stage->SetOnGround(transform.position_, time, gravity); // ステージの位置を確認し、空中に浮いていないか確認する 浮いていたら重力をかける
	VECTOR3 front = transform.position_ + VECTOR3(0, 0, 1) *  100 * MGetRotY(transform.rotation_.y);
	VECTOR3 back  = transform.position_ + VECTOR3(0, 0, 1) * -100 * MGetRotY(transform.rotation_.y);
	stage->CheckPush(transform.position_, front, distanceR); // ステージへのめり込みを確認する(前方)
	stage->CheckPush(transform.position_, back, distanceR);  // ステージへのめり込みを確認する(後方)

	// Actorどうしの当たり判定
}

bool GameMaster::IsCanAttackPlayer(Enemy* enemy)
{
	float min = (*enemy).GetDistanceR() + player->GetDistanceR(); // ぶつかる距離
	VECTOR3 aCap2 = (*enemy).GetTransform().position_ + VECTOR3(0, 1, 0) * 180 * (*enemy).GetTransform().GetRotationMatrix();
	VECTOR3 pCap2 = player->GetTransform().position_ + VECTOR3(0, 1, 0) * 180 * player->GetTransform().GetRotationMatrix();
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

float GameMaster::GetPlayerDistanceR()
{
	return player->GetDistanceR();
}
