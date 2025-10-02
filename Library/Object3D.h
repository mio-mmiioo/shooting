#pragma once
#include "../Library/GameObject.h"

class Transform {
public:
	// positionなどは他のファイルでも参照できるようにしたいからここに書いてる
	VECTOR3 position; // 位置
	VECTOR3 rotation; // 回転
	VECTOR3 scale;	  // 軸ごとの拡縮
	
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);
		scale = VECTOR3(1, 1, 1);
		rotMatrix_ = MGetIdent();
		localMatrix_ = MGetIdent();
	}
	~Transform() {};

	// 位置・回転・拡縮を使用した行列の計算結果を求める
	const MATRIX& MakeLocalMatrix() {
		MATRIX ms = MGetScale(scale);
		MATRIX mrx = MGetRotX(rotation.x);
		MATRIX mry = MGetRotY(rotation.y);
		MATRIX mrz = MGetRotZ(rotation.z);
		MATRIX mt = MGetTranslate(position);
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
	virtual void Update() override;
	virtual void Draw() override;
	Transform GetTransform() { return transform_; }
	virtual void SetMove(VECTOR3 toPosition, float angSpeed, float moveSpeed); // 移動したい場所, 回転スピード, 動くスピード
protected:
	int hModel_;
	Transform transform_;
};