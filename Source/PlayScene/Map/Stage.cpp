#include "Stage.h"
#include <assert.h>
#include "../../../Library/CsvReader.h"
#include "../Player/Player.h"
#include "../Actor/Enemy/Enemy.h"
#include "StageObject.h"
#include "DestructibleObject.h"
#include "../Actor/Actor.h"

namespace STAGE
{
	const float CHECK_GROUND_LENGTH = 500.0f;

	enum SORT_NUM
	{
		PLAYER,
		CHARA,
		OBJ,
		OBJ_D,
		MAX_OBJ
	};
}

Stage::Stage(int number)
{
	char filename[16];
	sprintf_s<16>(filename, "Stage%02d", number);
	//new StageObject(filename, VGet(0,0,0), VGet(0,0,0), VGet(1,1,1));
	ReadMappingData(filename);
}

Stage::~Stage()
{
	if (hModel_ > 0) {
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
}

bool Stage::CollideLineStageObject(const VECTOR3& pos1, const VECTOR3& pos2, VECTOR3* hit) const
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2-pos1)).Size();
	std::list<StageObject*> objs = FindGameObjects<StageObject>();
	for (StageObject* ob : objs) 
	{
		VECTOR3 ret;
		if (ob->Object3D::CollideLine(pos1, pos2, &ret)) 
		{
			found = true;
			VECTOR3 v = pos1 - ret;
			float len = v.Size();
			if (len < nowVal)
			{
				nowVal = len;
				now = ret;
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}
	return found;
}

void Stage::SetOnGround(VECTOR3& pos, float& time, VECTOR3 Gravity)
{
	VECTOR3 hit;
	if (CollideLine(pos + VECTOR3(0, STAGE::CHECK_GROUND_LENGTH, 0), pos + VECTOR3(0, -STAGE::CHECK_GROUND_LENGTH, 0), &hit))
	{
		pos = hit;
		if (time != 0)
		{
			time = 0;
		}
	}
	else
	{
		//空中なら落下処理
		time += Time::DeltaTime();
		pos -= Gravity * time * time;
	}
}

void Stage::CheckPush(VECTOR3& pos1, VECTOR3 pos2, float minDistance)
{
	VECTOR3 hit;
	VECTOR3 direction;
	if (CollideLine(pos1, pos2, &hit)) // 正面にオブジェクトがある
	{
		if (VSize(pos1 - hit) < minDistance) // めり込んでいる→めり込んでいる距離押し返す
		{
			direction = VNorm(hit - pos1); // 押し返す方向のベクトル
			pos1 -= direction * (minDistance - VSize(pos1 - hit)); // ( 押し返す方向 ) * ( 押し返したい距離 )
		}
	}
}

void Stage::ReadMappingData(std::string filename)
{
	const std::string folder = "data/stage/";
	CsvReader* csv = new CsvReader(folder+filename+".csv");
	for (int line = 0; line < csv->GetLines(); line++) {
		int sortNumber = csv->GetInt(line, 0);
		if (sortNumber == STAGE::SORT_NUM::PLAYER) {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			float ang = csv->GetFloat(line, 5);
			int hp = csv->GetInt(line, 6);
			new Player(pos, ang, hp);
		} else if (sortNumber == STAGE::SORT_NUM::CHARA) {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			float ang = csv->GetFloat(line, 5);
			int hp = csv->GetInt(line, 6);
			switch (csv->GetInt(line, 1)) {
			case 0:
				new Player(pos, ang, hp);
				break;
			case 1:
				new Enemy(pos, ang, hp);
				break;
			}
		} else if (sortNumber == STAGE::SORT_NUM::OBJ) {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			VECTOR rot = VECTOR3(csv->GetFloat(line, 5), csv->GetFloat(line, 6), csv->GetFloat(line, 7));
			VECTOR sca = VECTOR3(csv->GetFloat(line, 8), csv->GetFloat(line, 9), csv->GetFloat(line, 10));
			char file[16];
			sprintf_s<16>(file, "Stage_Obj%03d", csv->GetInt(line, 1));
			StageObject* obj = new StageObject(file, pos, rot, sca);
		}
		else if (sortNumber == STAGE::SORT_NUM::OBJ_D) {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			VECTOR rot = VECTOR3(csv->GetFloat(line, 5), csv->GetFloat(line, 6), csv->GetFloat(line, 7));
			VECTOR sca = VECTOR3(csv->GetFloat(line, 8), csv->GetFloat(line, 9), csv->GetFloat(line, 10));
			int hp = csv->GetInt(line, 11);
			char file[16];
			sprintf_s<16>(file, "Stage_Obj%03d", csv->GetInt(line, 1));
			DestructibleObject* obj = new DestructibleObject(file, pos, rot, sca, hp);
		}
	}
	delete csv;
}