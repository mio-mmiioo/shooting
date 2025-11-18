#pragma once
#include "../../../Library/Object3D.h"

// 移動可能なActorが、経路探索をするのに必要な情報をもらうためのクラス
class WayInfo : public Object3D 
{
public:
	WayInfo();
	~WayInfo();
	void Update();
	void Draw();

private:
	enum DIR { // 方向
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	enum MAP_NUM { // csvから読み込まれるマップの情報
		EMPTY,
		WALL,
		BRANCH,
		OBJECT_SPACE,
		MAX_MAP_NUM
	};
};
