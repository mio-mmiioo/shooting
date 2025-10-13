#include "Input.h"
#include "DxLib.h"

namespace Input {
	// キーボード取得関連
	const int KEY_MAX = 255;
	char curKey[KEY_MAX];		// 現在のキー
	char prevKey[KEY_MAX];		// 1つ前のキー 
	char keyDown[KEY_MAX];		// 押された瞬間 
	char keyUp[KEY_MAX];		// 離された瞬間 
	char KeyKeep[KEY_MAX];		// 押しっぱなし

	// マウスの状態
	int prevMouse = 0;			// 前のマウス状態
	int curMouse = 0;			// 現在のマウス状態

	// Xboxコントローラー関連

}

void Input::KeyStateUpdate()
{
	memcpy_s(prevKey, sizeof(char) * KEY_MAX, curKey, sizeof(char) * KEY_MAX);

	GetHitKeyStateAll(curKey); // 全てのキーの状態を取得 

	for (int i = 0; i < KEY_MAX; i++)
	{
		if (curKey[i] && prevKey[i]) KeyKeep[i]++;
		int keyXor = curKey[i] ^ prevKey[i];	// 前フレームと現フレームのxor
		if (keyXor) KeyKeep[i] = 0;
		keyDown[i] = keyXor & curKey[i];		// 押された瞬間 = (現在とkey_xorのAND) 
		keyUp[i] = keyXor & prevKey[i];			// 離された瞬間 = (前回とkey_xorのAND) 
	}

	// マウス関連
	prevMouse = curMouse;					// 前回のマウス状態を保存
	curMouse = GetMouseInput();				// マウスの状態を取得
}


bool Input::IsKeyUP(int keyCode)
{
	return(keyUp[keyCode]);
}

bool Input::IsKeyDown(int keyCode)
{
	return(keyDown[keyCode]);
}

int Input::IsKeepKeyDown(int keyCode)
{
	return(KeyKeep[keyCode]);
}


bool Input::IsButtonDown(int button)
{
	return(((prevMouse & button) == 0) && ((curMouse & button) != 0));
}

bool Input::IsButtonUP(int button)
{
	return(((prevMouse & button) != 0) && ((curMouse & button) == 0));
}

bool Input::IsButtonKeep(int button)
{
	return(((prevMouse & button) != 0) && ((curMouse & button) != 0));
}