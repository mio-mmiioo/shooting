#include "ResultScene.h"
#include "../../Library/Input.h"
#include "Result.h"
#include "Observer.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
	Observer::Release();
}

void ResultScene::Update()
{
	if (Input::IsKeyDown(KEY_INPUT_T) || Input::IsJoypadDown(XINPUT_BUTTON_B)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (Input::IsKeyDown(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void ResultScene::Draw()
{
	Result::Draw();
	DrawString(0, 0, "RESULT SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
