#pragma once
#include "../Actor.h"
#include "../Gun/Gun.h"

class Camera;

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;

	VECTOR3 GetWPointerPos() { return wPointerPos_; }
	VECTOR3 GetStartPos() { return startPos_; }
	int Attack() override; // 攻撃力を入れる -1 の時は攻撃してない

private:
	Camera* camera_;
	Gun* gun_;
	int mouseX_, mouseY_;
	VECTOR3 wPointerPos_; // スクリーン座標のポインターをワールド座標に変換したものを代入する変数
	VECTOR3 startPos_; // 銃の軌道の始点
	bool isHit_; // enemyなどのActorクラスとぶつかるかどうか

	// ポインターの画像
	int hImagePointer_;							// 標準時のポインター
	int imagePointerX_, imagePointerY_;			// 画像のサイズを代入する変数
	int hImagePointerHit_;						// 敵に銃弾が当たる場合のポインター
	int imagePointerHitX_, imagePointerHitY_;	// 画像のサイズを代入する変数
	int hImageReload_;							// リロードするときのポインター
	int imageReloadX_, imageReloadY_;			// 画像のサイズを代入する変数

	// 銃弾関連
	bool IsAttackInput(); // 銃によって攻撃の入力処理を変更する
	bool isAttack_;

	GUN::TYPE currentGun_;

};

