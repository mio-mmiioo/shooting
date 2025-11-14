#include "HP.h"
#include <assert.h>

namespace {
	int LEFT_TOP_X = 20;
	int LEFT_TOP_Y = 20;

	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 43;

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
	effectDamageImage_ = LoadGraph("data/image/effectDamageHp.png");
	assert(effectDamageImage_ > 0);

	damageTimer_ = 0.0f;
	hpRaitio_ = 0.0f;

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
			hpRaitio_ = 0.0f;
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
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * raitio), IMAGE_HEIGHT, barRedImage_, TRUE);
		break;

	case State::DAMAGE:
		timeRaitio_ = damageTimer_ / DAMAGE_TIME;
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * (raitio + hpRaitio_ * timeRaitio_)), IMAGE_HEIGHT, barRedImage_, TRUE);
		break;
	}

	DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * raitio), IMAGE_HEIGHT, barGreenImage_, TRUE);
}

void HP::AddHP(int addHp)
{
	addHp_ = std::abs(addHp);
	hpRaitio_ = (float)std::abs(addHp_) / (float)maxHp_ + hpRaitio_ * timeRaitio_;
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
