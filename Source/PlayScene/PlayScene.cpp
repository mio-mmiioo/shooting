#include "PlayScene.h"
#include "Map/Stage.h"
#include "Player/Camera.h"
#include "Development/Axis.h"
#include "../../Library/Light.h"
#include "../../Library/Shadow.h"

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));

	//new Axis();
	new Camera();
	new Stage(1);

	Light::Init();
	Shadow::Init(); // Stageの後にInitする

	//StartJoypadVibration(DX_INPUT_PAD1, 400, -1, -1); // つながってるか確認したくて振動させた
}

PlayScene::~PlayScene()
{
	Shadow::Release(); // シャドウの後処理
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