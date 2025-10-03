#include "Stage.h"
#include <assert.h>
#include "StageObject.h"
#include "../Player/Player.h"

Stage::Stage(int number)
{
	char filename[16];
	sprintf_s<16>(filename, "Stage%02d", number);
	new StageObject(filename, VGet(0,0,0), VGet(0,0,0), VGet(1,1,1));
	//ReadMappingData(filename);
}

Stage::~Stage()
{
	if (hModel_ > 0) {
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
}
void Stage::ReadMappingData(std::string filename)
{
	//VECTOR pos = VECTOR3(0, 50, 0);
	//float ang = 0;
	//int hp = 10;
	//new Player(pos, ang, hp);
}