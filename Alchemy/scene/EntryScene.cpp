#include "EntryScene.h"
#include "SceneMng.h"
#include <_DebugConOut.h>
#include <Input/PadState.h>
#include<Input/InputState.h>
#include<Input/INPUT_ID.h>
#include"GameScene.h"
#include<object/PlNum.h>
#include<NetWork/NetWork.h>

EntryScene::EntryScene()
{
	_fadespeed = 15;
	_fadecolor = 0;
	_entrnum = false;
	_cnt = 0;
	DrawInit();
	Init();
	lpNetWork.NetMode(NETMODE::HOST);						// πﬁΩƒ”∞ƒﬁê›íË ŒΩƒÇÕLaboÇÃ
	func = { &EntryScene::EntryStart };

}


EntryScene::~EntryScene()
{
}

UniqueBase EntryScene::Update(UniqueBase own)
{
	SetDrawBright(_fadecolor, _fadecolor, _fadecolor);

	for (int j = 0; j < 4; j++)
	{
		lpSceneMng.AddDrawQue({ _backImage[j],(320.0 * static_cast<double>((j + 1))) - 160.0,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
	}
	lpSceneMng.AddDrawQue({ _skyimage, lpSceneMng.ScreenCenter.x,208.0,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });
	lpSceneMng.AddDrawQue({ _entryBG,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });

	(this->*func)();

	return std::move(own);
}

void EntryScene::Init(void)
{
	_input = std::make_unique<PadState>(DX_INPUT_PAD1);
}

void EntryScene::EntryStart(void)
{
	if (_fadecolor < 255)
	{
		_fadecolor += _fadespeed;
	}
	else
	{
		func = &EntryScene::EntryNormal;
	}
}

void EntryScene::EntryNormal(void)
{
	(*_input).Update(_objList);

	if (_entrnum != 0)
	{
		lpNetWork.Connect();
		_cnt++;
		if (lpNetWork.Active())
		{
			auto mes = lpNetWork.GetMes(MES_TYPE::CONNECT);
			if (static_cast<MES_TYPE>(mes.check.type)!=MES_TYPE::CONNECT)
			{
				mes.connect.connectNum = static_cast<unsigned char>(lpNetWork.GetPlNum());
			}
			for (auto plNum = begin(PlNum()); plNum < static_cast<PlNum>(mes.connect.connectNum); ++plNum)
			{
				lpSceneMng.AddPlayerQue({ plNum,{lpSceneMng.ScreenSize.x / 2.0 + (100.0*static_cast<int>(plNum)),lpSceneMng.ScreenSize.y / 4.0}, 1 });
				lpSceneMng.AddDrawQue({ _entryImage[static_cast<int>(plNum)][((_cnt / 10) % 4)],(320.0 * (static_cast<double>(plNum) + 1.0)) - 160.0,lpSceneMng.ScreenCenter.y,0,1.2,0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
			}
		}
	}

	if ((*_input).btnState(INPUT_ID::BTN_START).first)
	{
		_entrnum = true;
	}
}

void EntryScene::DrawInit(void)
{
	_entryBG = LoadGraph("image/TitleBG.png");
	for (int i = 0; i < 4; i++)
	{
		_backImage[i] = LoadGraph("image/Woodback.png");
	}
	LoadDivGraph("image/Player_walk.png", 16, 4, 4, 266, 249, *_entryImage);
	_skyimage = LoadGraph("image/sky.png");
}
