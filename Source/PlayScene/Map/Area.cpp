#include "Area.h"
#include <vector>

namespace Area {
	std::vector<VECTOR3> nextPosition = {
		{ {   0, 0,    0} },
		{ { 500, 0,  500} },
		{ { 500, 0, -500} },
		{ {-500, 0, -500} },
		{ {-500, 0,  500} }
	};

	int nextPositionNumber = 0;
}


VECTOR3 Area::GetNextPosition()
{
	nextPositionNumber += 1;
	if (nextPositionNumber >= nextPosition.size())
	{
		nextPositionNumber = 1;
	}
	return Area::nextPosition[Area::nextPositionNumber];
}
