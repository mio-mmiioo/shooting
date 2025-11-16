#include "Player.h"
#include "Camera.h"
#include <assert.h>
#include "../../Map/Stage.h"
#include "../../../../Library/Input.h"
#include "../Gun/Gun.h"
#include "../HP.h"
#include "../Gun/Effect.h"
#include "../../GameMaster.h"
#include "../../../Sound.h"

namespace PLAYER
{
	VECTOR3 G = { 0, 9.8, 0 };
	float rotateSpeed = 3.0f;
	float moveSpeed = 5.0f;

	const VECTOR3 CAPSULE_POS1 = { 0,  50, 0 };
	const VECTOR3 CAPSULE_POS2 = { 0, 150, 0 };
	const float DISTANCE_R = 50.0f;
}

Player::Player(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hModel_ = MV1LoadModel("data/model/player02.mv1");
	assert(hModel_ > 0);

	// ポインター
	// 標準時のポインター
	hImagePointer_ = LoadGraph("data/image/pointer1.png");
	assert(hImagePointer_ > 0);
	GetGraphSize(hImagePointer_, &imagePointerX_, &imagePointerY_);
	// 敵に当たるときのポインター
	hImagePointerHit_ = LoadGraph("data/image/pointer2.png");
	assert(hImagePointer_ > 0);
	GetGraphSize(hImagePointer_, &imagePointerHitX_, &imagePointerHitY_);

	// リロードするときのポインター
	hImageReload_ = LoadGraph("data/image/reload.png");
	assert(hImageReload_ > 0);
	GetGraphSize(hImageReload_, &imageReloadX_, &imageReloadY_);

	// 銃を切り替えるときの画像
	hImageGunCircle_ = LoadGraph("data/image/gunCircle.png");
	assert(hImageGunCircle_ > 0);
	isChangeGun_ = false;

	GetMousePoint(&mouseX_, &mouseY_);

	goPosition_ = VECTOR3(0, 0, 0);
	isArrive_ = true;
	isHit_ = false;

	hp_ = new HP(hp);

	// 銃弾関連
	gun_ = new Gun();
	currentGun_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGun_); // 使用する銃の種類をセット
	isAttack_ = false;

	camera_ = FindGameObject<Camera>();
	stage_ = FindGameObject<Stage>();
	time_ = 0;

	gravity_ = PLAYER::G;
	distanceR_ = PLAYER::DISTANCE_R;

	SetDrawOrder(-1);
}

Player::~Player()
{
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
}

void Player::Update()
{
	isAttack_ = false;

	GetMousePoint(&mouseX_, &mouseY_);

	// 自動移動 他の処理書くために、一時的にコメントアウト
	{
		/*if (isArrive_ == false)
		{
			SetMove(goPosition_, 1.0f, 2.0f);

			if (VSize(goPosition_ - transform_.position_) < 50.0f)
			{
				isArrive_ = true;
			}
		}*/
	}
	
	// 手動回転 開発時のみ　他の処理書くために、一時的にコメントアウト
	{
		if (Input::IsKeepKeyDown(KEY_INPUT_D))
		{
			transform_.rotation_.y += PLAYER::rotateSpeed * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_A))
		{
			transform_.rotation_.y -= PLAYER::rotateSpeed * DegToRad;
		}
	}

	// 手動移動 開発時のみ　他の処理書くために、一時的にコメントアウト
	{
		VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
		velocity = VECTOR3(0, 0, 1) * PLAYER::moveSpeed * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

		if (Input::IsKeepKeyDown(KEY_INPUT_W))
		{
			transform_.position_ += velocity;
		}
		else if (Input::IsKeepKeyDown(KEY_INPUT_S))
		{
			transform_.position_ -= velocity;
		}
	}

	// 銃弾の入力処理
	{
		if (Input::IsMouseDown(MOUSE_INPUT_MIDDLE))
		{
			// 銃切り替えの処理
			// 画像を表示するためのmouseの位置を引数で渡す
			changeGunPosX_ = mouseX_;
			changeGunPosY_ = mouseY_;
		}
		if (Input::IsMouseKeep(MOUSE_INPUT_MIDDLE))
		{
			// 現在のマウスの位置からどこを選択しているのか割り出す
			isChangeGun_ = true;
			ChangeGun(mouseX_, mouseY_);
		}
		else
		{
			isChangeGun_ = false;
		}

		if (Input::IsMouseDown(MOUSE_INPUT_RIGHT) || Input::IsJoypadDown(XINPUT_BUTTON_RIGHT_SHOULDER))
		{
			gun_->ReloadBullet(); // リロードの処理
		}
		if (IsAttackInput() == true) // 銃の種類によって入力の判定が異なる 
		{
			if (gun_->OutBullet() == true) // 攻撃成功→true : 銃弾を発射する処理 エフェクト・音・振動もここで処理
			{
				isAttack_ = true;
			}
		}
	}

	// 銃弾の当たり判定
	{
		VECTOR ScreenPos = { (float)mouseX_, (float)mouseY_, 1.0f };
		wPointerPos_ = ConvScreenPosToWorldPos(ScreenPos);
		startPos_ = transform_.position_ + VECTOR3(0, 180, 0);
		if (GameMaster::IsBulletHitEnemy(startPos_, wPointerPos_) == true)
		{
			isHit_ = true;
		}
		else
		{
			isHit_ = false;
		}
	}

	// 当たり判定　敵と自分の距離を確認
	{
		// 雑にHPを減らす
		if (Input::IsKeyDown(KEY_INPUT_K))
		{
			hp_->AddHP(-2);
		}
	}

	GameMaster::CheckSetPosition(transform_, time_, gravity_, distanceR_);
	camera_->SetPlayerPosition(transform_);						 // プレイヤーの情報をカメラにセット
}

void Player::Draw()
{
	Object3D::Draw();

	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = { 0, 180, 0 };
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * 100 * MGetRotY(transform_.rotation_.y), GetColor(255, 255, 255));

	// 2Dの描画

	// 残弾数の表示
	gun_->Draw();

	DrawFormatString(10, 60, GetColor(255, 255, 255), "HP:%d", hp_->GetHP());

	// 銃の切り替え
	if (isChangeGun_ == true)
	{
		DrawCircleGauge(changeGunPosX_, changeGunPosY_, 100.0f, hImageGunCircle_, 0.0f, TRUE);
		return; // 銃を切り替えるときは、ポインターを描画しない
	}

	// ポインターの描画
	if (isHit_ == true)
	{
		DrawGraph(mouseX_ - imagePointerHitX_ / 2, mouseY_ - imagePointerHitY_ / 2, hImagePointerHit_, TRUE); // Actorに当たる
	}
	else
	{
		DrawGraph(mouseX_ - imagePointerX_ / 2, mouseY_ - imagePointerY_ / 2, hImagePointer_, TRUE); // 標準
	}

	if (gun_->GetReloadTimer() > 0)
	{
		float rate = (gun_->GetReloadTime() - gun_->GetReloadTimer()) / gun_->GetReloadTime() * 100; // (maxの時間 - 残り時間) / maxの時間 * 100 = 〇〇%
		DrawCircleGauge(mouseX_, mouseY_, 100.0, hImageReload_, rate);
	}
}

int Player::Attack()
{
	if (isAttack_ == true)
	{
		return gun_->GetAttack();
	}
	return -1;
}

void Player::ChangeGun(int currentMouseX, int currentMouseY)
{
	if (changeGunPosX_ <= currentMouseX)
	{
		currentGun_ = GUN::TYPE::HAND;
	}
	else if (changeGunPosX_ >= currentMouseX)
	{
		currentGun_ = GUN::TYPE::MACHINE;
	}

	gun_->SetGunType(currentGun_);
}

bool Player::IsAttackInput()
{
	switch (currentGun_)
	{
	case GUN::TYPE::HAND: // 一発ずつ発砲
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT) || Input::IsJoypadDown(XINPUT_BUTTON_B))
		{
			return true;
		}
		break;

	case GUN::TYPE::MACHINE: // 長押しで連射
		if (Input::IsMouseKeep(MOUSE_INPUT_LEFT) || Input::IsJoypadKeep(XINPUT_BUTTON_B))
		{
			return true;
		}
		break;
	}
	return false;
}
