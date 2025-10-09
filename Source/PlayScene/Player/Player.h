#pragma once
#include "../Actor.h"

class Camera;

class Player : public Actor
{
public:
	Player(const VECTOR3& position, float ang, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;
private:
	Camera* camera_;
	int mouseX_, mouseY_;
	VECTOR3 wPointerPos_; // スクリーン座標のポインターをワールド座標に変換したものを代入する変数
	bool isHit_; // enemyなどのActorクラスとぶつかるかどうか

	// ポインターの画像
	int hImagePointer_;							// 標準時のポインター
	int imagePointerX_, imagePointerY_;			// 画像のサイズを代入する変数
	int hImagePointerHit_;						// 敵に銃弾が当たる場合のポインター
	int imagePointerHitX_, imagePointerHitY_;	// 画像のサイズを代入する変数

	// 銃弾関連
	float reloadTimer_;

};

