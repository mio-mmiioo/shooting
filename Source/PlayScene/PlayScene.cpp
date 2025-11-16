#include "PlayScene.h"
#include "Actor/Player/Camera.h"
#include "Development/Axis.h"
#include "../../Library/Light.h"
#include "../../Library/Shadow.h"
#include "GameMaster.h"

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));

	new Camera();
	GameMaster::Init();
	Light::Init();
	Shadow::Init(); // Stage‚ÌŒã‚ÉInit‚·‚é
}

PlayScene::~PlayScene()
{
	GameMaster::Release();
	Shadow::Release(); // ƒVƒƒƒhƒE‚ÌŒãˆ—
}

void PlayScene::Update()
{
	GameMaster::Update();
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
}