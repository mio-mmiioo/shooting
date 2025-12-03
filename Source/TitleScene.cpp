#include "TitleScene.h"
#include <fstream>
#include "../Library/Input.h"
#include "Color.h"

using namespace std;

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(KEY_INPUT_P) || Input::IsJoypadDown(XINPUT_BUTTON_B)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (Input::IsKeyDown(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	extern const char* Version();
	DrawString(0, 20, Version(), Color::WHITE);
	DrawString(0, 0, "TITLE SCENE", Color::WHITE);
	DrawString(100, 400, "Push [P]Key To Play", Color::WHITE);
}
