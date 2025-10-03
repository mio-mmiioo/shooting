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
protected:
	int hModel_;
	Transform transform_;
	Object3D* parent_;
};