#pragma once
#include "../../../Library/Object3D.h"
#include <vector>

namespace WayInfo {
	void Init();
	void WayDraw(); // マップの情報を視覚的に描画
	void DrawVertex(); // 頂点リストの頂点を描画

	VECTOR3 SetVertexPosition(VECTOR3 position, int num);
	std::vector<VECTOR2> GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos); // ( 現在地、目的地 )
}

