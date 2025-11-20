#include "WayInfo.h"
#include "../../../Library/CsvReader.h"

struct vertex {
	VECTOR2 position; // 位置情報
	int distance; // 距離
	int number;
	bool isDicision; // 決定しているか
	std::vector<vertex> next; // つながってる頂点リスト
	std::vector<VECTOR2> posList; // 最短経路の道情報
};

struct way {
	VECTOR2 startPos = { 0, 0 };
	VECTOR2 endPos = { 0, 0 };
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

	void InitVertexList(); // 頂点情報リストを初期化
	bool CheckVertex(int x, int y); // 頂点ならtrue
	vertex FindStartVertex(); // 頂点リストの最初の頂点を求める
	void SetShortestWay(vertex start); // 最短経路を求める
	int GetCost(VECTOR2 startPos, VECTOR2 endPos); // 距離(cost)を求める
	std::vector<VECTOR2> GetShortestWay(VECTOR2 pos);

	const int MAX_DISTANCE = 5000;
	const int BOX_SIZE = 100;
	VECTOR2 dir_[4]; // 方向
	std::vector<std::vector<int>> wayInfo_; // 通れる場所の情報

	VECTOR2 startPos_; // 経路探索を開始したい位置
	VECTOR2 goalPos_; // 目的地
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

	startPos_ = VECTOR2(-1, -1);
	InitVertexList();
}

void WayInfo::WayDraw()
{
	VECTOR3 add = { 5000.0f, 0.0f, 5000.0f };
	int color = 0;
	for (int i = 0; i < wayInfo_.size(); i++)
	{
		for (int j = 0; j < wayInfo_[i].size(); j++)
		{
			VECTOR3 topLeft = VECTOR3(i * BOX_SIZE, 5.0f, j * BOX_SIZE);
			VECTOR3 topRight = VECTOR3(i * BOX_SIZE + BOX_SIZE, 5.0f, j * BOX_SIZE);
			VECTOR3 downLeft = VECTOR3(i * BOX_SIZE, 5.0f, j * BOX_SIZE + BOX_SIZE);
			VECTOR3 downRight = VECTOR3(i * BOX_SIZE + BOX_SIZE, 5.0f, j * BOX_SIZE + BOX_SIZE);

			if (wayInfo_[i][j] == 0)
			{
				color = GetColor(100, 255, 100);
			}
			else if (wayInfo_[i][j] == 1)
			{
				color = GetColor(0, 0, 0);
			}
			else if (wayInfo_[i][j] == 2)
			{
				color = GetColor(0, 0, 255);
			}
			else
			{
				color = GetColor(100, 100, 100);
			}
			DrawTriangle3D(topLeft - add, topRight - add, downRight - add, color, TRUE);
			DrawTriangle3D(downRight - add, downLeft - add, topLeft - add, color, TRUE);
		}
	}
}

std::vector<VECTOR2> WayInfo::GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos)
{
	// 道情報の初期化
	for (int i = 0; i < vertexList_.size(); i++)
	{
		vertexList_[i].distance = MAX_DISTANCE;
		vertexList_[i].isDicision = false;
		vertexList_[i].posList.clear();
	}

	// スタートの位置を代入
	startPos_ = VECTOR2(currentPos.x, currentPos.y);
	vertex start = FindStartVertex(); // 最初の位置を distance = 0 にする
	SetShortestWay(start);

	// whileでcheckVertexListが存在する間回す
	while (!checkVertexList_.empty())
	{
		SetShortestWay(checkVertexList_.front());
		checkVertexList_.erase(checkVertexList_.begin());
	}

	// goalPosに一番近い頂点をvertexListから探す
	std::vector<VECTOR2> ret = GetShortestWay(VECTOR2(goalPos.x, goalPos.y));

	return ret;
}

void WayInfo::InitVertexList()
{
	// 分岐点を頂点の番号に変更 頂点リストに頂点を追加
	for (int y = 0; y < wayInfo_.size(); y++)
	{
		for (int x = 0; x < wayInfo_.size(); x++)
		{
			if (CheckVertex(x, y) == true) // 分岐点
			{
				wayInfo_[y][x] = MAP_NUM::BRANCH;
				vertex v = { VECTOR2(x, y), MAX_DISTANCE, vertexList_.size(), false, std::vector<vertex>() };
				vertexList_.push_back(v);
			}
		}
	}

	// nextをセット
	for (int i = 0; i < vertexList_.size(); i++)
	{
		for (int direction = 0; direction < DIR::MAX_DIR; direction++)
		{
			VECTOR2 check = vertexList_[i].position + dir_[direction];

			int distance = 1;
			// 距離(cost)を求める
			if (wayInfo_[(int)check.y][(int)check.x] != MAP_NUM::WALL && wayInfo_[(int)check.y][(int)check.x] != MAP_NUM::OBJECT_SPACE)
			{
				while (wayInfo_[(int)check.y][(int)check.x] != MAP_NUM::BRANCH)
				{
					check = check + dir_[direction];
					distance += 1;
				}

				for (int j = 0; j < vertexList_.size(); j++)
				{
					if (vertexList_[j].position.x == check.x && vertexList_[j].position.y == check.y)
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

bool WayInfo::CheckVertex(int x, int y)
{
	bool ret = false;
	if (x == 0 || y == 0 || x == 100 - 1 || y == 100 - 1)
	{
		return ret;
	}

	if (wayInfo_[y][x] != MAP_NUM::EMPTY)
	{
		return ret;
	}

	int counter = 0;	
	bool checkDir[DIR::MAX_DIR];

	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		int checkX = x + (int)dir_[i].x;
		int checkY = y + (int)dir_[i].y;
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
				ret == true;
			}
		}
	}

	return ret;
}

vertex WayInfo::FindStartVertex()
{
	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (startPos_.x == vertexList_[i].position.x && startPos_.y == vertexList_[i].position.y)
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
		if (vertexList_[i].position.x == start.position.x && vertexList_[i].position.y == start.position.y)
		{
			vertexList_[i].isDicision = true;
			vertexList_[i].posList.push_back(start.position);
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
					if (vertexList_[j].position.x == vertexList_[start.number].next[i].position.x && vertexList_[j].position.y == vertexList_[start.number].next[i].position.y)
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
			for (int j = sortMinDistance.size() - 2; j >= 0; j--)
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

			for (int j = checkVertexList_.size() - 2; j >= 0; j--)
			{
				if (checkVertexList_[j].distance > checkVertexList_[j + 1].distance)
				{
					std::swap(checkVertexList_[j].distance, checkVertexList_[j + 1].distance);
				}
			}
		}
	}
}

int WayInfo::GetCost(VECTOR2 startPos, VECTOR2 endPos)
{
	for (int i = 0; i < wayList_.size(); i++)
	{
		if (wayList_[i].startPos.x == startPos.x && wayList_[i].startPos.y == startPos.y)
		{
			if (wayList_[i].endPos.x == endPos.x && wayList_[i].endPos.y == endPos.y)
			{
				return wayList_[i].cost;
			}
		}
	}
	return MAX_DISTANCE;
}

std::vector<VECTOR2> WayInfo::GetShortestWay(VECTOR2 pos)
{
	int x = pos.x / BOX_SIZE;
	int y = pos.y / BOX_SIZE;

	for (int i = 0; i < vertexList_.size(); i++)
	{
		if (vertexList_[i].position.x == x)
		{
			if (vertexList_[i].position.y == y)
			{
				int checkNum = vertexList_[i].posList.size() - 1;
				while (vertexList_[i].posList[checkNum].x == vertexList_[i].posList[checkNum - 1].x &&
					vertexList_[i].posList[checkNum].y == vertexList_[i].posList[checkNum - 1].y)
				{
					vertexList_[i].posList.pop_back();
					checkNum -= 1;
				}

				return vertexList_[i].posList;
			}
		}
	}

	return std::vector<VECTOR2>();
}
