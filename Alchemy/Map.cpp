#include "Map.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <DxLib.h>
#include  <_DebugConOut.h>
#include <ImageMng.h>
#include <UI_Mng.h>
#include <object/Player.h>
std::unique_ptr<Map, Map::MapDeleter>Map::sInstans(new Map);

Map::Map()
{
	LoadMap();
	_miniMapDrawFlag = true;
}


Map::~Map()
{
}

void Map::Init(void)
{
	lpImageMng.GetID({ IMG::BG ,STATE::HOLD }, "image/miniMap.png");
	lpImageMng.GetID({ IMG::BG_CURSOR,STATE::NORMAL }, "image/cursor.png", { 6,6 }, {4,1});
	_mapScreen = MakeScreen(static_cast<int>(lpSceneMng.WorldSize.x), static_cast<int>(lpSceneMng.WorldSize.y), true);
	_miniMapScreen = MakeScreen(static_cast<int>(lpSceneMng.WorldSize.x / 8), static_cast<int>(lpSceneMng.WorldSize.y / 8), true);
	ImageKey key = { IMG::BG,STATE::NORMAL };
	SetDrawScreen(_mapScreen);
	ClsDrawScreen();
	for (int y = 0; y < MAP_CHIP_Y; y++)
	{
		for (int x = 0; x < MAP_CHIP_X; x++)
		{
			DrawGraph(CHIP_SIZE_X*x, CHIP_SIZE_Y*y, lpImageMng.GetID(key)[_mapData[y][x] - 1], true);
		}
	}
	key = { IMG::BG,STATE::HOLD };
	SetDrawScreen(_miniMapScreen);
	ClsDrawScreen();
	DrawGraph(0, 0, lpImageMng.GetID(key)[0], true);
	SetDrawScreen(DX_SCREEN_BACK);
}

const int Map::mapScreen(void) const
{
	return _mapScreen;
}

void Map::miniMapDrawFlag(void)
{
	_miniMapDrawFlag = !_miniMapDrawFlag;
}

void Map::ChangeChip(Vector2Dbl pos, double rad, int num)
{
	if (!_miniMapDrawFlag)
	{
		return;
	}
	_chipData.emplace_back(MapQueT{ static_cast<int>(pos.x / CHIP_SIZE_X),static_cast<int>(pos.y / CHIP_SIZE_Y),rad,num });
}

void Map::Draw(void)
{
	if (_miniMapDrawFlag)
	{
		ImageKey key = { IMG::BG,STATE::HOLD };
		SetDrawScreen(_miniMapScreen);
		ClsDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
		DrawGraph(0, 0, lpImageMng.GetID(key)[0], true);
		key = { IMG::BG_CURSOR,STATE::NORMAL };
		//
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		int x, y,num;
		double rad;
		for (auto data : _chipData)
		{
			std::tie(x, y, rad, num) = data;
			DrawRotaGraph(
				x * 2,
				y * 2,
				1.0,
				rad,
				lpImageMng.GetID(key)[static_cast<int>(num)],
				true
				);
		}
		UI_Mng::GetInstance().AddUIQue({ 960,540,_miniMapScreen});
		if (Player::GetpCnt() > 1)
		{
			UI_Mng::GetInstance().AddUIQue({ 320,540,_miniMapScreen });
		}
	}
	_chipData.clear();
}

bool Map::LoadMap(void)
{
	std::fstream file("data/mapLayer01.csv");
	// file���J��
	if (!file.is_open())
	{
		// �J����ĂȂ������ꍇ�I��
		AST();
		return false;
	}
	// file�̓ǂݍ���
	while (!file.eof())
	{
		// 1�s�ǂݍ���
		std::string buf;
		file >> buf;

		std::vector<int> strList;				// 1�s����ؽ�
		std::istringstream strbuf(buf);		// �������ذ�
		std::string token;						// 1�ٕ��̕�����

		while (std::getline(strbuf, token, ','))
		{
			strList.emplace_back(std::stoi(token));
		}
		_mapData.emplace_back(strList);
	}
	file.close();
	return true;
}
