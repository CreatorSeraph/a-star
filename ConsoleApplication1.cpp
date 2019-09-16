#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct cNode
{
	bool isWall;
	size_t h;//목적지까지의 예상코스트
	size_t g;//현재까지의 코스트
	size_t f;//현재까지의 코스트 + 예상코스트

	int rootNodeIdx;//어디서 왔니
	bool isFindedNode;//탐색한 노드니?

	cNode(char c)//이부분 작성해서 기본생성자가 삭제됨
		:isWall(c == 'x')
		,h(0), g(0), f(0)
		,rootNodeIdx(0), isFindedNode(false)
	{
	}
};

class cMap
{
	friend ostream& operator<<(ostream& o, const cMap& map)
	{
		size_t count = 0;
		for (auto iter : map.map)
		{
			o << (iter.isWall ? 'x' : 'o');
			if (++count == map.width)
			{
				o << endl;
				count = 0;
			}
		}
		return o;
	}
public:
	vector<cNode> map;
	size_t width;
	size_t height;
public:
	cMap(size_t _width, initializer_list<char> _list)
		:width(_width), height(_list.size() / _width)
	{
		for (auto iter : _list)
		{
			map.emplace_back(iter);
		}
	}

	bool Find(size_t startIdx, size_t endIdx)
	{
		int mapSize = width * height;
		for (int i = 0; i < mapSize; ++i)
		{
			auto& iter = map[i];
			if (iter.isWall) continue;

			iter.h = abs(static_cast<int>(i % width) - static_cast<int>(endIdx % width))
				+ abs(static_cast<int>(i / width) - static_cast<int>(endIdx / width));
			iter.g = 99999999;
			iter.f = iter.h + iter.g;
			iter.isFindedNode = false;
			iter.rootNodeIdx = -1;
		}

		vector<size_t> targetIdxList;//탐색대상 번호 리스트

		targetIdxList.push_back(startIdx);
		map[startIdx].isFindedNode = true;
		map[startIdx].g = 0;

		bool result = false;
		while (!targetIdxList.empty())
		{
			size_t minFVal = 999999999;
			int minFIdx = -1;
			for (auto idx : targetIdxList)
			{
				if (map[idx].f < minFVal)
				{
					minFVal = map[idx].f;
					minFIdx = idx;
				}
			}

			targetIdxList.erase(find(targetIdxList.begin(), targetIdxList.end(), minFIdx));
			map[minFIdx].isFindedNode = true;

			auto x = minFIdx % width;
			auto y = minFIdx / width;
			vector<size_t> adjIdxList;//인접한 인덱스 리스트
			if (y != 0)
				adjIdxList.push_back(minFIdx - width);
			if (y != height - 1)
				adjIdxList.push_back(minFIdx + width);
			if (x != 0)
				adjIdxList.push_back(minFIdx - 1);
			if (x != width - 1)
				adjIdxList.push_back(minFIdx + 1);

			for (auto idx : adjIdxList)
			{
				auto& iter = map[idx];
				if (iter.isFindedNode) continue;//이미 빗금쳐진노드는 탐색대상이 아니다
				if (iter.isWall) continue;//벽도 당연히 아니다. 병신인가?
				if(iter.g > map[minFIdx].g + 1)
					iter.g = map[minFIdx].g + 1;//코스트 + 1
				iter.f = iter.g + iter.h;
				iter.rootNodeIdx = minFIdx;
				if (idx == endIdx)
					result = true;
				if (find(targetIdxList.begin(), targetIdxList.end(), idx) == targetIdxList.end())
					targetIdxList.push_back(idx);
			}
			if (result)
				break;
		}
		return result;
	}
};

int main()
{
	cMap m(5, {
		'o', 'o', 'o', 'x', 'o',
		'o', 'x', 'o', 'x', 'o',
		'o', 'x', 'o', 'o', 'o',
		'o', 'o', 'o', 'x', 'o',
		'x', 'x', 'x', 'x', 'o',
		'o', 'o', 'o', 'o', 'o',
	});

	cout << m << endl;
	bool result = m.Find(0, 29);

	int idx = 29;
	do
	{
		cout << idx << "\t";
		idx = m.map[idx].rootNodeIdx;
	} while (idx != 0);
}