#include "Player.h"
#include "Camera.h"
#include <assert.h>
#include "../Map/Stage.h"
#include "../../../Library/Input.h"
#include "../Gun/Gun.h"

namespace PLAYER
{
	VECTOR3 G = { 0, 9.8, 0 };
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

	GetMousePoint(&mouseX_, &mouseY_);

	isHit_ = false;

	// 銃弾関連
	gun_ = new Gun();
	currentGun_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGun_); // 使用する銃の種類をセット
	isAttack_ = false;

	camera_ = FindGameObject<Camera>();
	stage_ = FindGameObject<Stage>();
	time_ = 0;
}

Player::~Player()
{
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	//これ書いた方が増える
	//if (hImagePointer_ > 0)
	//{
	//	DeleteGraph(hImagePointer_);
	//	hImagePointer_ = -1;
	//}
	//if (hImagePointerHit_ > 0)
	//{
	//	DeleteGraph(hImagePointerHit_);
	//	hImagePointerHit_ = -1;
	//}
	//if (hImagePointerHit_ > 0)
	//{
	//	DeleteGraph(hImagePointerHit_);
	//	hImagePointerHit_ = -1;
	//}
	//if (hImageReload_ > 0)
	//{
	//	DeleteGraph(hImageReload_);
	//	hImageReload_ = -1;
	//}
}

void Player::Update()
{
	isAttack_ = false;

	GetMousePoint(&mouseX_, &mouseY_);

	VECTOR ScreenPos = { (float)mouseX_, (float)mouseY_, 1.0f };
	wPointerPos_ = ConvScreenPosToWorldPos(ScreenPos);

	//回転
	{
		if (Input::IsKeepKeyDown(KEY_INPUT_D))
		{
			transform_.rotation_.y += 3 * DegToRad;
		}
		if (Input::IsKeepKeyDown(KEY_INPUT_A))
		{
			transform_.rotation_.y -= 3 * DegToRad;
		}
	}

	//移動

	VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
	velocity = VECTOR3(0, 0, 1) * 5.0f * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

	if (Input::IsKeepKeyDown(KEY_INPUT_W))
	{
		transform_.position_ += velocity;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_S))
	{
		transform_.position_ -= velocity;
	}

	// 当たり判定　Stage→Actor の順で確認している
	{
		VECTOR3 hit;
		startPos_ = transform_.position_ + VECTOR3(0, 180, 0);
		//DrawLine3D(startPos, wPointerPos_, GetColor(255, 255, 255));

		if (stage_->CollideLine(startPos_, wPointerPos_, &hit))
		{
			// stageObjectに当たる場合の処理
		}

		if (Actor::CollideLine(startPos_, wPointerPos_, &hit))
		{
			// Actorに当たる場合の処理
			isHit_ = true;
		}
		else
		{
			isHit_ = false;
		}
	}

	// 銃弾
	if (Input::IsKeyDown(KEY_INPUT_6))
	{
		currentGun_ = GUN::TYPE::HAND;
		gun_->SetGunType(currentGun_);
	}
	else if (Input::IsKeyDown(KEY_INPUT_7))
	{
		currentGun_ = GUN::TYPE::MACHINE;
		gun_->SetGunType(currentGun_);
	}

	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT) || Input::IsJoypadDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		gun_->ReloadBullet(); // リロードの処理
	}
	else if (Input::IsMouseDown(MOUSE_INPUT_LEFT) || Input::IsJoypadDown(XINPUT_BUTTON_B))
	{
		if (gun_->current.reloadTimer <= 0) // リロード中じゃない→撃てる
		{
			gun_->OutBullet(); // 銃弾を発射する処理 エフェクト・音・振動もここで処理
			isAttack_ = true;
		}
	}

	stage_->SetOnGround(transform_.position_, time_, PLAYER::G); // ステージの位置を確認し、空中に浮いていないか確認する
	camera_->SetPlayerPosition(transform_);						 // プレイヤーの情報をカメラにセット

}

void Player::Draw()
{
	Object3D::Draw();

	//// 向いてる方向を示す　これカメラ変更しなくなったら消すこと
	VECTOR3 addPlayerHeight = { 0, 180, 0 };
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * 100 * MGetRotY(transform_.rotation_.y), GetColor(255, 255, 255));

	// 2Dの描画

	// 残弾数の表示
	gun_->Draw();

	// ポインターの描画
	if (isHit_ == true)
	{
		DrawGraph(mouseX_ - imagePointerHitX_ / 2, mouseY_ - imagePointerHitY_ / 2, hImagePointerHit_, TRUE); // Actorに当たる
	}
	else
	{
		DrawGraph(mouseX_ - imagePointerX_ / 2, mouseY_ - imagePointerY_ / 2, hImagePointer_, TRUE); // 標準
	}

	if (gun_->current.reloadTimer > 0)
	{
		float rate = (gun_->current.reloadTime - gun_->current.reloadTimer) / gun_->current.reloadTime * 100; // (maxの時間 - 残り時間) / maxの時間 * 100 = 〇〇%
		DrawCircleGauge(mouseX_, mouseY_, 100.0, hImageReload_, rate, 1.0, 0, 0);
	}
}
