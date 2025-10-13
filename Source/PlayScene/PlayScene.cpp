#include "PlayScene.h"
#include "Map/Stage.h"
#include "Player/Camera.h"
#include "Development/Axis.h"
#include "../../Library/Right.h"
#include "../../Library/Shadow.h"

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));

	//new Axis();
	new Camera();
	new Stage(1);

	Right::Init();
	Shadow::Init(); // StageÇÃå„Ç…InitÇ∑ÇÈ

	StartJoypadVibration(DX_INPUT_PAD1, 400, -1, -1); // Ç¬Ç»Ç™Ç¡ÇƒÇÈÇ©ämîFÇµÇΩÇ≠ÇƒêUìÆÇ≥ÇπÇΩ
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}