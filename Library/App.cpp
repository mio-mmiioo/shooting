#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include "Input.h"
#include "Shadow.h"
#include <DxLib.h>
#include "../Source/Sound.h"

void AppInit()
{
	ObjectManager::Init();
	SceneManager::Init();
	Time::Init();
	Sound::InitSE();
}

void AppUpdate()
{
	Input::KeyStateUpdate();
	SceneManager::Update();
	ObjectManager::Update();
	Shadow::Update(); // オブジェクトを描画し始める前に処理したい
}

void AppDraw()
{
	Time::Refresh();
	ObjectManager::Draw();
	Shadow::Draw(); // オブジェクトを描画後に処理したい
	SceneManager::Draw();
}

void AppRelease()
{
	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
}

bool AppIsExit()
{
	return SceneManager::IsExit();
}