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
		player->SetPosition(SetMove(player->GetTransform(), Area::GetCurrentPosition(), 3.0f, 2.0f));
	}
	//if (player->GetTransform().position_.x != Area::GetCurrentPosition().x || player->GetTransform().position_.z != Area::GetCurrentPosition().z)
	//{
	//	// ゆっくりと移動させる関数を呼び出す
	//	SetMove(player->GetTransform(), Area::GetCurrentPosition(), 3.0f, 2.0f);
	//	
	//	//player->SetPosition(Area::GetNextPosition());
	//}
}

VECTOR3 GameMaster::SetMove(Transform currentTransform, VECTOR3 toPosition, float angSpeed, float moveSpeed)
{
	VECTOR3 toGo = toPosition - currentTransform.position_;

	VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(currentTransform.rotation_.y);//正面
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(currentTransform.rotation_.y);//右 回転を見るのに使ってる
	VECTOR3 ret;
	if (VDot(front, toGo.Normalize()) >= cos(angSpeed))
	{
		currentTransform.rotation_.y = atan2f(toGo.x, toGo.z);
	}
	else if (VDot(right, toGo) > 0)
	{
		currentTransform.rotation_.y += angSpeed;
	}
	else
	{
		currentTransform.rotation_.y -= angSpeed;
	}

	currentTransform.position_ += VECTOR3(0, 0, moveSpeed) * MGetRotY(currentTransform.rotation_.y);
	ret = currentTransform.position_;
	return ret;
}