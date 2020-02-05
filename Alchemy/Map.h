#pragma once

#include <memory>
#include <vector>
#include <tuple>
#include "object/PlNum.h"
#include <scene\SceneMng.h>

#define lpMap Map::GetInstans()
#define CHIP_SIZE_X 16
#define CHIP_SIZE_Y 16
#define MAP_CHIP_X 160
#define MAP_CHIP_Y 90
#define MINI_MAP_POS_X (lpSceneMng.ScreenSize.x-(lpSceneMng.ScreenSize.x / 8) * 2-CHIP_SIZE_X)
#define MINI_MAP_POS_Y (lpSceneMng.ScreenSize.y-(lpSceneMng.ScreenSize.y / 8) * 2-CHIP_SIZE_Y)

enum class MAP_QUE
{
	X,												// X����
	Y,												// Y����
	RAD,											// �p�x
	TYPE,											// ��ڲ԰���ʗp�@���Pl_NON�Ƃ��łǂ��ł�����
};

using MapQueT = std::tuple<int, int, double,int>;

class Map
{
public:
	void Draw(void);
	static Map& GetInstans(void)
	{
		return *sInstans;
	}
	void Init(void);								// ������
	const int mapScreen(void)const;
	void miniMapDrawFlag(void);
	void ChangeChip(Vector2Dbl pos,double rad,int num);
private:
	struct MapDeleter
	{
		void operator()(Map* map)const
		{
			delete map;
		}
	};
	static std::unique_ptr<Map, MapDeleter>sInstans;
	Map();
	~Map();
	std::vector<MapQueT> _chipData;					// ��ϯ�ߗpchip
	int _mapScreen;									// ϯ�ߑS�̐}
	int _miniMapScreen;								// ��ϯ��
	bool _miniMapDrawFlag;							// ��ϯ�ߕ`�悷�邩�ǂ���
	std::vector<std::vector<int>> _mapData;			// ϯ���ް�
	bool LoadMap(void);								// csv����̓ǂݍ���
};

