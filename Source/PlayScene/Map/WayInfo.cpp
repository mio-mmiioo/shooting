#include "WayInfo.h"
#include "../../../Library/CsvReader.h"

namespace WayInfo{
	enum DIR { // 方向
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	enum MAP_NUM { // csvから読み込まれるマップの情報
		EMPTY,
		WALL,
		BRANCH,
		OBJECT_SPACE,
		MAX_MAP_NUM
	};

	const int MAX_DISTANCE = 5000;
	VECTOR2 dir_[4]; // 方向
	std::vector<std::vector<int>> wayInfo_; // 通れる場所の情報

	void InitVertexList(); // 頂点情報リストを初期化
	bool CheckVertex(int x, int y); // 頂点ならtrue

	VECTOR2 startPos_; // 経路探索を開始したい位置
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
	sprintf_s<64>(filename, "data/stage%02d.csv", 1);
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
			if (wayInfo_[(int)check.y][(int)check.x] != MAP_NUM::WALL)
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
	int counter = 0;
	bool ret = false;
	
	bool checkDir[DIR::MAX_DIR];

	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		int checkX = x + (int)dir_[i].x;
		int checkY = y + (int)dir_[i].y;
		if (wayInfo_[y][x] == MAP_NUM::EMPTY || wayInfo_[y][x] == MAP_NUM::BRANCH)
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

	return false;
}
