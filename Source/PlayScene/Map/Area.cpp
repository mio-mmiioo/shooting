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
}


VECTOR3 Area::GetNextPosition()
{
	return VECTOR3();
}
