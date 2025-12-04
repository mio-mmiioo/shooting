#pragma once
//#include "../../../../Library/Object3D.h"
#include "../Actor.h"
#include "../Gun/Gun.h"

class Camera;
class Stage;
class HP;

struct image
{
	int hImage;
	int halfWidth;
	int halfHeight;
};

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;

	int Attack(); // 攻撃力を入れる -1 の時は攻撃してない
	void SetToGo(VECTOR3 goPosition) { goPosition_ = goPosition; }

	void SetPrevVretexPosition(VECTOR3 position) { prevVertexPosition_ = position; }
	VECTOR3 GetPrevVertexPosition() { return prevVertexPosition_; }
	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	bool GetArrive() { return isArrive_; }
	void Attacked(int atackPower);

private:
	void SetImage(image& i, std::string path);		 // 画像のセット
	void DevelopmentInput(); // 開発時のみ使用する、キー入力の処理
	Camera* camera_;
	Stage* stage_;
	Gun* gun_;
	HP* HP_;

	float time_;
	int mouseX_, mouseY_;
	VECTOR3 wPointerPos_; // スクリーン座標のポインターをワールド座標に変換したものを代入する変数
	VECTOR3 startPos_; // 銃の軌道の始点

	bool isHit_; // enemyなどのActorクラスに銃弾がぶつかるかどうか

	// 移動関連
	VECTOR3 prevVertexPosition_; // 前回の頂点の位置
	VECTOR3 goPosition_; // プレイヤーが次に向かう場所 目的地までに通る場所
	VECTOR3 endPosition_; // プレイヤーの目的地
	bool isArrive_;		 // 一度到着したことを確認する

	// 照準の画像関連
	image aiming_;		// 照準の画像
	image hitAiming_;	// 照準が何かに当たるときの画像
	image reload_;		// リロードの画像

	// 銃の切り替え関連
	void ChangeGun(int currentMouseX, int currentMouseY); // 銃を切り替える処理
	bool isChangeGun_;  // 銃を切り替え中か trueなら切り替えてる最中
	int changeGunPosX_; // 銃の切り替え表示の座標x
	int changeGunPosY_; // 銃の切り替え表示の座標y
	int hImageGunCircle_;


	// 銃弾関連
	bool IsAttackInput(); // 銃によって攻撃の入力処理を変更する
	bool isAttack_;

	GUN::TYPE currentGun_;

};

