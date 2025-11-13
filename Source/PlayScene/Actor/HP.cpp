#include "HP.h"
#include <assert.h>

namespace {
	int LEFT_TOP_X = 20;
	int LEFT_TOP_Y = 20;
}

HP::HP(int hp)
{
	hp_ = hp;
	maxHp_ = hp;

	// ‰æ‘œ“Ç‚Ýž‚Ý
	barFrameImage_ = LoadGraph("data/image/hpBarFrame.png");
	assert(barFrameImage_ > 0);
	barGreenImage_ = LoadGraph("data/image/hpBarGreen.png");
	assert(barGreenImage_ > 0);
	barRedImage_ = LoadGraph("data/image/hpBarRed.png");
	assert(barRedImage_ > 0);
}

HP::~HP()
{
}

void HP::Update()
{
}

void HP::Draw()
{
	float raitio = (float)hp_ / (float)maxHp_;
	DrawGraph(LEFT_TOP_X, LEFT_TOP_Y, barFrameImage_, TRUE);
	DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * raitio), 43, barRedImage_, TRUE);
	DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * raitio), 43, barGreenImage_, TRUE);
}

void HP::AddHP(int addHp)
{
	hp_ += addHp;
}
