#include "Shadow.h"
#include "DxLib.h"
#include "Light.h"
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

	SetShadowMapLightDirection(hShadowMap, Light::GetLightDirection());
	SetShadowMapDrawArea(hShadowMap, minPosition, maxPosition);
	ShadowMap_DrawSetup(hShadowMap);
	ObjectManager::Draw();
	ShadowMap_DrawEnd();
}

void Shadow::Update()
{
	ShadowMap_DrawSetup(hShadowMap);
	// モデルを描画
	ObjectManager::Draw(); // これ書き方もう少しいい方法ある気がする
	ShadowMap_DrawEnd();
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


