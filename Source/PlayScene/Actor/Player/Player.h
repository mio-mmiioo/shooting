#pragma once
#include "../../../../Library/Object3D.h"
#include "../Gun/Gun.h"

class Camera;
class Enemy;
class Stage;

class Player : public Object3D
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;

	int Attack(); // 攻撃力を入れる -1 の時は攻撃してない
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }

private:
	Camera* camera_;
	Stage* stage_;
	Gun* gun_;
	std::list<Enemy*> enemy_;
	std::list<Enemy*> hitEnemy_;

	float time_;
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

