#include "PlayScene.h"
#include "Actor/Player/Camera.h"
#include "Development/Axis.h"
#include "../../Library/Light.h"
#include "../../Library/Shadow.h"
#include "../../Library/Input.h"
#include "GameMaster.h"

PlayScene::PlayScene()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(50, 100, -300), VECTOR3(0, 0, 0));

	new Camera();
	GameMaster::Init();
	Light::Init();
	Shadow::Init(); // StageÇÃå„Ç…InitÇ∑ÇÈ
}

PlayScene::~PlayScene()
{
	GameMaster::Release();
	Shadow::Release(); // ÉVÉÉÉhÉEÇÃå„èàóù
}

void PlayScene::Update()
{
	GameMaster::Update();
	if (Input::IsKeyDown(KEY_INPUT_R) || Input::IsJoypadDown(XINPUT_BUTTON_Y)) {
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
}