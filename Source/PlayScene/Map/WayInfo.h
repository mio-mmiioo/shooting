#pragma once
#include "../../../Library/Object3D.h"
#include <vector>

struct vertex {
	VECTOR2 position; // 位置情報
	int distance; // 距離
	int number;
	bool isDicision; // 決定しているか
	std::vector<vertex> next; // つながってる頂点リスト
	std::vector<VECTOR2> posList; // 最短経路の道情報
};

struct way {
	VECTOR2 startPos = { 0, 0 };
	VECTOR2 endPos = { 0, 0 };
	int cost = 0;
};

namespace WayInfo {
	void Init();

	std::vector<VECTOR2> GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos); // ( 現在地、目的地 )
}

