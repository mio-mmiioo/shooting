#pragma once
#include "../Library/GameObject.h"

class Transform {
public:
	// positionなどは他のファイルでも参照できるようにしたいからここに書いてる
	VECTOR3 position_; // 位置
	VECTOR3 rotation_; // 回転
	VECTOR3 scale_;	  // 軸ごとの拡縮
	
	Transform() {
		position_ = VECTOR3(0, 0, 0);
		rotation_ = VECTOR3(0, 0, 0);
		scale_ = VECTOR3(1, 1, 1);
		rotMatrix_ = MGetIdent();
		localMatrix_ = MGetIdent();
	}

	~Transform() {};

	// 位置・回転・拡縮を使用した行列の計算結果を求める
	const MATRIX& MakeLocalMatrix() {
		MATRIX ms = MGetScale(scale_);
		MATRIX mrx = MGetRotX(rotation_.x);
		MATRIX mry = MGetRotY(rotation_.y);
		MATRIX mrz = MGetRotZ(rotation_.z);
		MATRIX mt = MGetTranslate(position_);
		rotMatrix_ = ms * mrz * mrx * mry;
		localMatrix_ = rotMatrix_ * mt;
		return localMatrix_;
	}

	// 位置・回転・拡縮を使用した行列を変えす　※計算をしてない
	const MATRIX& GetLocalMatrix() const { return localMatrix_; }
	
	// 回転・拡縮を使用した行列を変えす　※計算をしてない
	const MATRIX& GetRotationMatrix() const { return rotMatrix_; }

private:
	MATRIX rotMatrix_;
	MATRIX localMatrix_;
};

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override; // ここで重力をかけていきたい
	virtual void Draw() override;
	Transform GetTransform() { return transform_; }
	void SetParent(Object3D* _parent) { parent_ = _parent; }
	virtual bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const;
	VECTOR3 GetGravity() { return gravity_; }
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }
	float GetDistanceR() { return distanceR_; }
	void SetMove(VECTOR3 toPosition);

protected:
	int hModel_; // 本当に描画するモデル
	int hitModel_; // 当たり判定用のモデル
	bool isDraw_; // 描画するかどうか true→描画する
	Transform transform_;
	Object3D* parent_;

	VECTOR3 gravity_; // 重力

	// 当たり判定関連
	float distanceR_; // 当たり判定に使用する半径
	float moveSpeed_;
	float rotateSpeed_; // 回転速度
};