/// <summary>
/// CoGフレームワーク
/// WinMain()から始まります
/// </summary>
/// <author>N.Hanai</author>
/// 
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__);
#else
#define DBG_NEW new
#endif

#define IMGUI 1
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
	ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	//SetHookWinProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT /*CALLBACK*/
	//{
	//	// DxLibとImGuiのウィンドウプロシージャを両立させる
	//	SetUseHookWinProcReturnValue(FALSE);
	//	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	//});
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
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif
	return 0;				// ソフトの終了 
}