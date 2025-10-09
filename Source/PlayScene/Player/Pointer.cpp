#include "Pointer.h"
#include "DxLib.h"

VECTOR3 wPointerPos;

namespace POINTER
{
	void Update()
	{
		int mouseX;
		int mouseY;

		GetMousePoint(&mouseX, &mouseY);

		VECTOR ScreenPos = { (float)mouseX, (float)mouseY, 1.0f }; // z はDxLibのリファレンスをもとに入れてる　わからなくなったら確認してみて

		wPointerPos = ConvScreenPosToWorldPos(ScreenPos);
	}

	void Draw()
	{
	}

	VECTOR3 GetWorldPointerPos()
	{
		return wPointerPos;
	}

}