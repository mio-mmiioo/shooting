#include "Stage.h"
#include <assert.h>
#include "../../../Library/CsvReader.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "StageObject.h"

Stage::Stage(int number)
{
	char filename[16];
	sprintf_s<16>(filename, "Stage%02d", number);
	new StageObject(filename, VGet(0,0,0), VGet(0,0,0), VGet(1,1,1));
	ReadMappingData(filename);
}

Stage::~Stage()
{
	if (hModel_ > 0) {
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
}

bool Stage::CollideLine(const VECTOR3& pos1, const VECTOR3& pos2, VECTOR3* hit) const
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2-pos1)).Size();
	std::list<StageObject*> objs = FindGameObjects<StageObject>();
	for (StageObject* ob : objs) {
		VECTOR3 ret;
		if (ob->CollideLine(pos1, pos2, &ret)) {
			found = true;
			VECTOR3 v = pos1 - ret;
			float len = v.Size();
			if (len < nowVal) {
				nowVal = len;
				now = ret;
			}
		}
	}
	if (hit != nullptr) {
		*hit = now;
	}
	return found;
}

void Stage::ReadMappingData(std::string filename)
{
	const std::string folder = "data/stage/";
	CsvReader* csv = new CsvReader(folder+filename+".csv");
	for (int line = 0; line < csv->GetLines(); line++) {
		std::string com = csv->GetString(line, 0);
		if (com == "PLAYER") {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			float ang = csv->GetFloat(line, 5);
			int hp = csv->GetInt(line, 6);
			new Player(pos, ang, hp);
		} else if (com == "CHARA") {
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
		} else if (com == "OBJ") {
			VECTOR pos = VECTOR3(csv->GetFloat(line, 2), csv->GetFloat(line, 3), csv->GetFloat(line, 4));
			VECTOR rot = VECTOR3(csv->GetFloat(line, 5), csv->GetFloat(line, 6), csv->GetFloat(line, 7));
			VECTOR sca = VECTOR3(csv->GetFloat(line, 8), csv->GetFloat(line, 9), csv->GetFloat(line, 10));
			char file[16];
			sprintf_s<16>(file, "Stage_Obj%03d", csv->GetInt(line, 1));
			StageObject* obj = new StageObject(file, pos, rot, sca);
		}
	}
	delete csv;
}