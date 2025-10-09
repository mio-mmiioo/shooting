#pragma once

namespace BULLET
{
	void Init();
	void Update();
	void Draw();
	void DrawRemainingSetting(int currentRemainingSetting);
	void DrawReloadCircle(int mouseX, int mouseY, float timer);
	void OutBullet();
	void AddBullet(int addNumber);
}