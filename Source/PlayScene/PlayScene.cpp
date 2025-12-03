#include "PlayScene.h"
#include "Actor/Player/Camera.h"
#include "Development/Axis.h"
#include "../../Library/Light.h"
#include "../../Library/Shadow.h"
#include "GameMaster.h"
#include "../Color.h"

PlayScene::PlayScene()
{
	//new Axis();
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
	GameMaster::Draw();
	DrawString(0, 0, "PLAY SCENE", Color::WHITE);
}