#include "PlayScene.h"
#include "Map/Stage.h"
#include "Player/Camera.h"
#include "Player/Player.h" // プレイヤーが描画されているか確認するように入れてる
#include "Development/Axis.h"

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));
	new Axis();
	new Camera();
	new Player(VECTOR3(0, 0, 0), 0, 10);
	//new Stage(1);

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