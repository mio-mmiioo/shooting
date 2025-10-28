#include "Area.h"
#include <vector>
#include "Stage.h"

namespace Area {
	std::vector<VECTOR3> nextPosition = {
		{ {   0, 0,    0} },
		{ { 500, 0,  500} },
		{ { 500, 0, -500} },
		{ {-500, 0, -500} },
		{ {-500, 0,  500} }
	};

	int nextPositionNumber = 0;
	VECTOR3 currentPosition;
}

void Area::SetStage()
{
	new Stage(nextPositionNumber);

}

VECTOR3 Area::GetCurrentPosition()
{
	return Area::currentPosition;
}

void Area::SetNextPosition()
{
	nextPositionNumber += 1;
	if (nextPositionNumber >= nextPosition.size())
	{
		nextPositionNumber = 1;
	}
	currentPosition = nextPosition[nextPositionNumber];
}
