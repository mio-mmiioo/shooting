#include "HP.h"
#include <assert.h>

namespace {
	int LEFT_TOP_X = 20;
	int LEFT_TOP_Y = 20;

	const float DAMAGE_TIME = 1.5f;
}

HP::HP(int hp)
{
	addHp_ = 0;
	hp_ = hp;
	maxHp_ = hp;

	// 画像読み込み
	barFrameImage_ = LoadGraph("data/image/hpBarFrame.png");
	assert(barFrameImage_ > 0);
	barGreenImage_ = LoadGraph("data/image/hpBarGreen.png");
	assert(barGreenImage_ > 0);
	barRedImage_ = LoadGraph("data/image/hpBarRed.png");
	assert(barRedImage_ > 0);

	damageTimer_ = 0.0f;

	state_ = State::NORMAL;
	
}

HP::~HP()
{
}

void HP::Update()
{
	switch (state_)
	{
	case State::NORMAL:
		break;
	case State::HEAL:
		state_ = State::NORMAL;
		break;
	case State::DAMAGE:
		damageTimer_ -= Time::DeltaTime();
		if (damageTimer_ <= 0.0f)
		{
			addHp_ = 0;
			state_ = State::NORMAL;
		}

		break;
	}
}

void HP::Draw()
{
	float raitio = (float)hp_ / (float)maxHp_;
	DrawGraph(LEFT_TOP_X, LEFT_TOP_Y, barFrameImage_, TRUE);
	// NORMAL,HEALは対応が同じ、ダメージは赤いゲージを見せる
	switch (state_)
	{
	case State::NORMAL:
	case State::HEAL:
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * raitio), 43, barRedImage_, TRUE);
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * raitio), 43, barGreenImage_, TRUE);
		break;
	case State::DAMAGE:

		float hpRaitio = (float)addHp_ / maxHp_;
		float tRaitio = damageTimer_ / DAMAGE_TIME;

		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * (raitio + hpRaitio * tRaitio)), 43, barRedImage_, TRUE);
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(400 * raitio), 43, barGreenImage_, TRUE);
		break;
	}
}

void HP::AddHP(int addHp)
{
	addHp_ += std::abs(addHp);
	hp_ += addHp;
	if (hp_ > maxHp_) // 最大値より大きくなっていた場合
	{
		hp_ = maxHp_;
	}
	
	if (addHp > 0)
	{
		state_ = State::HEAL;
	}
	else if (addHp < 0)
	{
		state_ = State::DAMAGE;
		damageTimer_ = DAMAGE_TIME;
	}
}
