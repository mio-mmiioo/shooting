#pragma once

namespace Color
{
	const int BLACK = GetColor(0, 0, 0);
	const int WHITE = GetColor(255, 255, 255);
	const int RED = GetColor(255, 0, 0);
	const int GREEN = GetColor(0, 255, 0);
	const int BLUE = GetColor(0, 0, 255);

	// WayInfoŠÖ˜A
	const int EMPTY = GetColor(100, 255, 100);
	const int WALL = BLACK;
	const int BRANCH = BLUE;
	const int OTHER = GetColor(100, 100, 100);

	// EffectŠÖ˜A
	const int HAND = WHITE;
	const int MACHINE = BLACK;
}
