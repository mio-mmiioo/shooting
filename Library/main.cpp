/// <summary>
/// CoGフレームワーク
/// WinMain()から始まります
/// </summary>
/// <author>N.Hanai</author>
/// 

#include <DxLib.h>
#include "App.h"
#include "../Source/Screen.h"

#define CoGVersion "4.5"

const char* Version() {
	return CoGVersion;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

	SetMainWindowText(Screen::WINDOW_NAME);
	SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
	SetBackgroundColor(200, 200, 200);
	ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	AppInit();

	while (true) {

		AppUpdate();

		if (ProcessMessage() == -1 || AppIsExit())
		{
			break;
		}
		ScreenFlip();
		ClearDrawScreen();
		AppDraw();

		RefreshDxLibDirect3DSetting();
	}

	AppRelease();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}