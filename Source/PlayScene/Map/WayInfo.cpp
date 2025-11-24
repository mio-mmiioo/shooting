#include "WayInfo.h"
#include "../../../Library/CsvReader.h"

struct point {
	int x;
	int z;
};

struct vertex {
	point position = {0, 0}; // 位置情報
	int distance = -1; // 距離
	int number = -1;
	bool isDicision = false; // 決定しているか
	std::vector<vertex> next; // つながってる頂点リスト
	std::vector<VECTOR2> posList; // 最短経路の道情報
};

struct way {
	point startPos = { 0, 0 };
	point endPos = { 0, 0 };
	int cost = 0;
};

namespace WayInfo{
	// 方向
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};
	
	// csvから読み込まれるマップの情報
	enum MAP_NUM {
		EMPTY,
		WALL,
		BRANCH,
		OBJECT_SPACE,
		MAX_MAP_NUM
	};

	void InitVertexList();							// 頂点情報リストを初期化
	bool CheckVertex(point p);						// 頂点ならtrue
	vertex FindStartVertex();						// 頂点リストの最初の頂点を求める
	void SetShortestWay(vertex start);				// 最短経路を求める
	int GetCost(point startPos, point endPos);		// 距離(cost)を求める
	std::vector<VECTOR3> GetShortestWay(point pos);

	const int MAX_DISTANCE = 5000;
	const VECTOR3 ADD_WAY_INFO_POS = { 5000, 0, 5000 };
	const VECTOR3 ADD_HALF_BOX_POS = { BOX_SIZE / 2, 0, BOX_SIZE / 2 };
	point dir_[4]; // 方向
	std::vector<std::vector<int>> wayInfo_; // 通れる場所の情報

	point startPos_; // 経路探索を開始したい位置
	point goalPos_; // 目的地
	std::vector<vertex> vertexList_; // 頂点情報のリスト
	std::vector<vertex> checkVertexList_; // 確認する頂点リスト 最短経路を求めるときに使用
	std::vector<way> wayList_; // 道情報のリスト
}

void WayInfo::Init()
{
	dir_[RIGHT] = { 1,  0 };
	dir_[DOWN] = { 0,  1 };
	dir_[LEFT] = { -1,  0 };
	dir_[UP] = { 0, -1 };

	char filename[64];
	sprintf_s<64>(filename, "data/stage/wayInfo/wayInfo%02d.csv", 0);
	wayInfo_.clear();

	// ステージデータの読み込み
	CsvReader* csv = new CsvReader(filename);
	for (int line = 0; line < csv->GetLines(); line++) {
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++) {
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		wayInfo_.push_back(mapLine);
	}
	delete csv;

	startPos_ = { -1,-1 };
	InitVertexList();
}

void WayInfo::WayDraw()
{
	int color = 0;
	for (int y = 0; y < wayInfo_.size(); y++)
	{
		for (int x = 0; x < wayInfo_[y].size(); x++)
		{
			VECTOR3 topLeft = VECTOR3(y * (float)BOX_SIZE, 5.0f, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 topRight = VECTOR3(y * (float)BOX_SIZE + (float)BOX_SIZE, 5.0f, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downLeft = VECTOR3(y * (float)BOX_SIZE, 5.0f, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downRight = VECTOR3(y * (float)BOX_SIZE + (float)BOX_SIZE, 5.0f, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;

			if (wayInfo_[x][y] == 0)
			{
				color = GetColor(100, 255, 100);
			}
			else if (wayInfo_[x][y] == 1)
			{
				color = GetColor(0, 0, 0);
			}
			else if (wayInfo_[x][y] == 2)
			{
				color = GetColor(0, 0, 255);
			}
			else
			{
				color = GetColor(100, 100, 100);
			}
			DrawTriangle3D(topLeft, topRight, downRight, color, TRUE);
			DrawTriangle3D(downRight, downLeft, topLeft, color, TRUE);
		}
	}
}

void WayInfo::DrawVertex()
{
	DrawSphere3D(ADD_WAY_INFO_POS * -1.0f, 40, 20, GetColor(255,255,255), GetColor(255, 255, 255), TRUE);
	VECTOR3 pos;
	int color = 0;
	for (int i = 0; i < vertexList_.size(); i++)
	{
		pos = VECTOR3(vertexList_[i].position.x * BOX_SIZE, 0.0f, vertexList_[i].position.z * BOX_SIZE);
		if (i == 0)
		{
			color = GetColor(255, 255, 255);
		}
		else
		{
			color = GetColor(0, 0, 0);
		}

		DrawSphere3D(pos - ADD_WAY_INFO_POS + ADD_HALF_BOX_POS, 40, 20, color, color, TRUE);
	}
}

VECTOR3 WayInfo::SetVertexPosition(VECTOR3 position, int num)
{
	VECTOR3 ret = position;
	ret.x = (float)(vertexList_[num].position.x * BOX_SIZE);
	ret.z = (float)(vertexList_[num].position.z * BOX_SIZE);

	return ret - ADD_WAY_INFO_POS + ADD_HALF_BOX_POS;
}

int WayInfo::CheckVertexNum(VECTOR3 position)
{
	position += ADD_WAY_INFO_POS;
	int x = position.x / BOX_SIZE;
	int z = position.z / BOX_SIZE;
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (x == vertexList_[i].position.x && z == vertexList_[i].position.z)
		{
			return i;
		}
	}
	return -1;
}

bool WayInfo::IsVertexPosition(VECTOR3 position)
{
	position += ADD_WAY_INFO_POS;
	int x = position.x / BOX_SIZE;
	int z = position.z / BOX_SIZE;
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (x == vertexList_[i].position.x && z == vertexList_[i].position.z)
		{
			return true;
		}
	}

	return false;
}

std::vector<VECTOR3> WayInfo::GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos)
{
	// 道情報の初期化
	for (int i = 0; i < vertexList_.size(); i++)
	{
		vertexList_[i].distance = MAX_DISTANCE;
		vertexList_[i].isDicision = false;
		vertexList_[i].posList.clear();
	}

	// スタートの位置を代入
	startPos_ = point{ (int)(currentPos.x / BOX_SIZE + wayInfo_.size() / 2), (int)(currentPos.z / BOX_SIZE + wayInfo_.size() / 2) };
	vertex start = FindStartVertex(); // 最初の位置を distance = 0 にする
	SetShortestWay(start);

	// whileでcheckVertexListが存在する間回す
	while (!checkVertexList_.empty())
	{
		SetShortestWay(checkVertexList_.front());
		checkVertexList_.erase(checkVertexList_.begin());
	}

	// goalPosに一番近い頂点をvertexListから探す
	goalPos_ = point{ (int)(goalPos.x / BOX_SIZE + wayInfo_.size() / 2), (int)(goalPos.z / BOX_SIZE + wayInfo_.size() / 2) };
	std::vector<VECTOR3> ret = GetShortestWay(goalPos_);

	return ret;
}

void WayInfo::InitVertexList()
{
	// 分岐点を頂点の番号に変更 頂点リストに頂点を追加
	for (int y = 0; y < wayInfo_.size(); y++)
	{
		for (int x = 0; x < wayInfo_.size(); x++)
		{
			if (CheckVertex(point{ x, y }) == true) // 分岐点
			{
				wayInfo_[y][x] = MAP_NUM::BRANCH;
				vertex v = { point{x, y}, MAX_DISTANCE, (int)vertexList_.size(), false, std::vector<vertex>() };
				vertexList_.push_back(v);
			}
		}
	}

	// nextをセット
	for (int i = 0; i < vertexList_.size(); i++)
	{
		for (int direction = 0; direction < DIR::MAX_DIR; direction++)
		{
			point check = { vertexList_[i].position.x + dir_[direction].x, vertexList_[i].position.z + dir_[direction].z };

			int distance = 1;
			// 距離(cost)を求める
			if (wayInfo_[check.z][check.x] != MAP_NUM::WALL && wayInfo_[check.z][check.x] != MAP_NUM::OBJECT_SPACE)
			{
				while (wayInfo_[check.z][check.x] != MAP_NUM::BRANCH)
				{
					check.x = check.x + dir_[direction].x;
					check.z = check.z + dir_[direction].z;
					distance += 1;
				}

				for (int j = 0; j < vertexList_.size(); j++)
				{
					if (vertexList_[j].position.x == check.x && vertexList_[j].position.z == check.z)
					{
						vertexList_[i].next.push_back(vertexList_[j]);
						wayList_.push_back(way{ vertexList_[i].position, vertexList_[j].position, distance });
						break;
					}
				}
			}
		}
	}
}

bool WayInfo::CheckVertex(point p)
{
	bool ret = false;
	if (p.x == 0 || p.z == 0 || p.x == 100 - 1 || p.z == 100 - 1)
	{
		return ret;
	}

	if (wayInfo_[p.z][p.x] != MAP_NUM::EMPTY)
	{
		return ret;
	}

	int counter = 0;	
	bool checkDir[DIR::MAX_DIR];

	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		int checkX = p.x + (int)dir_[i].x;
		int checkY = p.z + (int)dir_[i].z;
		if (wayInfo_[checkY][checkX] == MAP_NUM::EMPTY || wayInfo_[checkY][checkX] == MAP_NUM::BRANCH)
		{
			checkDir[i] = true;
			counter += 1;
		}
		else
		{
			checkDir[i] = false;
		}
	}

	if (counter > 2)
	{
		ret = true;
	}

	// 直角に曲がる
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		if (i != DIR::MAX_DIR - 1)
		{
			if (checkDir[i] == checkDir[i + 1])
			{
				ret = true;
			}
		}
		else
		{
			if (checkDir[i] == checkDir[0])
			{
				ret = true;
			}
		}
	}

	return ret;
}

vertex WayInfo::FindStartVertex()
{
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (startPos_.x == vertexList_[i].position.x && startPos_.z == vertexList_[i].position.z)
		{
			vertexList_[i].distance = 0;
			vertexList_[i].isDicision = true;
			return vertexList_[i];
		}
	}
	return vertex();
}

void WayInfo::SetShortestWay(vertex start)
{
	// 今確認中の頂点を決定済みにする
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == start.position.x && vertexList_[i].position.z == start.position.z)
		{
			vertexList_[i].isDicision = true;
			vertexList_[i].posList.push_back(VECTOR2((float)start.position.x, (float)start.position.z));
		}
	}

	// 次の場所に距離(cost)を入れる
	for (int i = 0; i < vertexList_[start.number].next.size(); i++)
	{
		int checkDistance = vertexList_[start.number].distance + GetCost(vertexList_[start.number].position, vertexList_[start.number].next[i].position);
		if (vertexList_[start.number].next[i].distance > checkDistance)
		{
			for (int j = 0; j < vertexList_.size(); j++)
			{
				if (vertexList_[j].isDicision == false)
				{
					if (vertexList_[j].position.x == vertexList_[start.number].next[i].position.x && vertexList_[j].position.z == vertexList_[start.number].next[i].position.z)
					{
						vertexList_[j].distance = checkDistance;
						vertexList_[j].posList.resize(vertexList_[start.number].posList.size());
						vertexList_[j].posList.assign(vertexList_[start.number].posList.begin(), vertexList_[start.number].posList.end());
					}
				}
			}
		}
	}

	// 現時点で最も近い場所を探す
	{
		std::vector<vertex> sortMinDistance;
		for (int i = 0; i < vertexList_[start.number].next.size(); i++)
		{
			if (vertexList_[vertexList_[start.number].next[i].number].isDicision == false)
			{
				sortMinDistance.push_back(vertexList_[start.number].next[i]);
			}
			// サイズが2以上ならソートする
			for (int j = (int)sortMinDistance.size() - 2; j >= 0; j--)
			{
				if (sortMinDistance[j].distance > sortMinDistance[j + 1].distance)
				{
					std::swap(sortMinDistance[j], sortMinDistance[j + 1]);
				}
			}
		}

		for (int i = 0; i < sortMinDistance.size(); i++)
		{
			checkVertexList_.push_back(sortMinDistance[i]);

			for (int j = (int)checkVertexList_.size() - 2; j >= 0; j--)
			{
				if (checkVertexList_[j].distance > checkVertexList_[j + 1].distance)
				{
					std::swap(checkVertexList_[j].distance, checkVertexList_[j + 1].distance);
				}
			}
		}
	}
}

int WayInfo::GetCost(point startPos, point endPos)
{
	for (int i = 0; i < wayList_.size(); i++)
	{
		if (wayList_[i].startPos.x == startPos.x && wayList_[i].startPos.z == startPos.z)
		{
			if (wayList_[i].endPos.x == endPos.x && wayList_[i].endPos.z == endPos.z)
			{
				return wayList_[i].cost;
			}
		}
	}
	return MAX_DISTANCE;
}

std::vector<VECTOR3> WayInfo::GetShortestWay(point pos)
{
	std::vector<VECTOR3> ret;
	// 最終的な経路を探す
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == pos.x && vertexList_[i].position.z == pos.z)
		{
			int checkNum = (int)vertexList_[i].posList.size() - 1;
			if (checkNum > 1)
			{
				while (vertexList_[i].posList[checkNum].x == vertexList_[i].posList[checkNum - 1].x &&
					vertexList_[i].posList[checkNum].y == vertexList_[i].posList[checkNum - 1].y)
				{
					vertexList_[i].posList.pop_back();
					checkNum -= 1;
				}
			}

			for (int j = 0; j < vertexList_[i].posList.size(); j++)
			{
				VECTOR3 v = { vertexList_[i].posList[j].x * BOX_SIZE, 0.0f, vertexList_[i].posList[j].y * BOX_SIZE };
				v -= ADD_WAY_INFO_POS - ADD_HALF_BOX_POS;
				v.y += 5.0f;
				ret.push_back(v);
			}
			break;
		}
	}
	return ret;
}
