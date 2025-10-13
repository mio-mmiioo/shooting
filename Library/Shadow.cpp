#include "Shadow.h"
#include "DxLib.h"
#include "Right.h"
#include "ObjectManager.h"

namespace Shadow {
	int hShadowMap; // シャドウマップのハンドル
	VECTOR minPosition; // 影をつけるエリアの最小ポジション
	VECTOR maxPosition; // 影をつけるエリアの最大ポジション
}

void Shadow::Init()
{
	hShadowMap = MakeShadowMap(8192, 8192); // 影をつけるためにセット これプレイヤーの周りだけにした方がよい気がする
	minPosition = { -4096, -4096, -4096 };
	maxPosition = {  4096,  4096,  4096 };

	if (0 != SetShadowMapLightDirection(hShadowMap, Right::GetRightDirection())) // ０：成功
	{
		// ミス
	}
	if (0 != SetShadowMapDrawArea(hShadowMap, minPosition, maxPosition)) // ０：成功
	{
		// ミス
	}
	if (0 != ShadowMap_DrawSetup(hShadowMap)) // ０：成功
	{
		// ミス
	}
	// モデルを描画
	ObjectManager::Draw(); // これ書き方もう少しいい方法ある気がする
	ShadowMap_DrawEnd();
}

void Shadow::Update()
{
	SetUseShadowMap(0, hShadowMap);
}

void Shadow::Draw()
{
	SetUseShadowMap(0, -1); // シャドウマップのハンドルを解除
}

void Shadow::Release()
{
	DeleteShadowMap(hShadowMap);
}


