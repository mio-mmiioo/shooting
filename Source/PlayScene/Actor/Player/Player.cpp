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
#include "../../../Color.h"

namespace PLAYER
{
	VECTOR3 G = { 0, 9.8, 0 };
	float ROTATE_SPEED = 3.0f;
	float MOVE_SPEED = 5.0f;

	const VECTOR3 CAPSULE_POS1 = { 0.0f,  50.0f, 0.0f };
	const VECTOR3 CAPSULE_POS2 = { 0.0f, 150.0f, 0.0f };
	const float DISTANCE_R = 50.0f;

	// 開発時のみ
	const float DIRECTION_LENGTH = 100.0f;
}

Player::Player(const VECTOR3& position, float ang, int hp)
{
	transform_.position_ = position;
	hModel_ = MV1LoadModel("data/model/player02.mv1");
	assert(hModel_ > 0);

	MV1SetupCollInfo(hModel_);
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);

	// 照準( aiming )
	SetImage(aiming_, "data/image/pointer1.png");	 // 標準時の照準
	SetImage(hitAiming_, "data/image/pointer2.png"); // 敵に当たるときの照準
	SetImage(reload_, "data/image/reload.png");		 // リロードするときの照準

	// 銃を切り替えるときの画像
	hImageGunCircle_ = LoadGraph("data/image/gunCircle.png");
	assert(hImageGunCircle_ > 0);
	isChangeGun_ = false;

	GetMousePoint(&mouseX_, &mouseY_);

	goPosition_ = VECTOR3(0, 0, 0);
	isArrive_ = true;
	isHit_ = false;

	HP_ = new HP(hp);

	// 銃弾関連
	gun_ = new Gun();
	currentGun_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGun_); // 使用する銃の種類をセット
	isAttack_ = false;

	camera_ = FindGameObject<Camera>();
	time_ = 0;

	gravity_ = PLAYER::G;
	distanceR_ = PLAYER::DISTANCE_R;
	moveSpeed_ = PLAYER::MOVE_SPEED;
	rotateSpeed_ = PLAYER::ROTATE_SPEED;

	SetPlayer(this);
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
	if (HP_->GetHP() <= 0)
	{
		GameMaster::PlayerDeath();
	}

	isAttack_ = false;

	GetMousePoint(&mouseX_, &mouseY_);
	
	// 手動移動など
	DevelopmentInput(); // 開発時のみ使用する入力処理

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
		startPos_ = transform_.position_ + LOOK_HEIGHT;
		if (GameMaster::IsBulletHit(startPos_, wPointerPos_) == true)
		{
			isHit_ = true;
		}
		else
		{
			isHit_ = false;
		}
	}

	// 位置情報の更新
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);

	GameMaster::CheckSetPosition(transform_, time_, gravity_, distanceR_);
	camera_->SetPlayerPosition(transform_);						 // プレイヤーの情報をカメラにセット
}

void Player::Draw()
{
	Object3D::Draw();

	// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = LOOK_HEIGHT * transform_.GetRotationMatrix();
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * PLAYER::DIRECTION_LENGTH * transform_.GetRotationMatrix(), Color::WHITE);

	// 2Dの描画

	// 残弾数の表示
	gun_->Draw();
	// 銃の切り替え
	if (isChangeGun_ == true)
	{
		DrawCircleGauge(changeGunPosX_, changeGunPosY_, 100.0f, hImageGunCircle_, 0.0f, TRUE);
		return; // 銃を切り替えるときは、ポインターを描画しない
	}

	// 照準の描画
	if (isHit_ == true)
	{
		DrawGraph(mouseX_ - hitAiming_.halfWidth, mouseY_ - hitAiming_.halfHeight, hitAiming_.hImage, TRUE); // Actorに当たる
	}
	else
	{
		DrawGraph(mouseX_ - aiming_.halfWidth, mouseY_ - aiming_.halfHeight, aiming_.hImage, TRUE); // 標準
	}

	if (gun_->GetReloadTimer() > 0)
	{
		float rate = (gun_->GetReloadTime() - gun_->GetReloadTimer()) / gun_->GetReloadTime() * 100; // (maxの時間 - 残り時間) / maxの時間 * 100 = 〇〇%
		DrawCircleGauge(mouseX_, mouseY_, 100.0, reload_.hImage, rate);
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

void Player::Attacked(int atackPower)
{
	HP_->AddHP(atackPower);
}

void Player::SetImage(image& i, std::string path)
{
	i.hImage = LoadGraph(path.c_str());
	assert(i.hImage > 0);
	GetGraphSize(i.hImage, &i.halfWidth, &i.halfHeight);
	i.halfWidth = i.halfWidth / 2;
	i.halfHeight = i.halfHeight / 2;
}

void Player::DevelopmentInput()
{
	// 手動回転 開発時のみ
	{
		if (Input::IsKeepKeyDown(KEY_INPUT_D))
		{
			transform_.rotation_.y += rotateSpeed_ * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_A))
		{
			transform_.rotation_.y -= rotateSpeed_ * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_X))
		{
			transform_.rotation_.x -= rotateSpeed_ * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_Z))
		{
			transform_.rotation_.z -= rotateSpeed_ * DegToRad;
		}
	}

	// 手動移動 開発時のみ
	{
		VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
		velocity = VECTOR3(0, 0, 1) * moveSpeed_ * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

		if (Input::IsKeepKeyDown(KEY_INPUT_W))
		{
			transform_.position_ += velocity;
		}
		else if (Input::IsKeepKeyDown(KEY_INPUT_S))
		{
			transform_.position_ -= velocity;
		}
	}
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
