#include "ResultScene.h"
#include"TitleScene.h"
#include "SceneMng.h"
#include <_DebugConOut.h>
#include <Input/PadState.h>
#include<Input/InputState.h>
#include<Input/INPUT_ID.h>
#include<UI_Mng.h>
#include <Camera.h>



ResultScene::ResultScene()
{
	TRACE("Ø»ÞÙÄ¼°Ý\n");
	_cnt = 0;
	DrawInit();
	_objList.clear();
	lpCamera.ReSetList();
	Init();
}


ResultScene::~ResultScene()
{
}

UniqueBase ResultScene::Update(UniqueBase own)
{
	(*_input).Update(_objList);
	lpUI_Mng.UI_ReSet();
	if ((*_input).btnState(INPUT_ID::BTN_START).first)
	{
		SetDrawBright(255, 255, 255);
		return std::make_unique<TitleScene>();
	}


	SetDrawBright(_cnt, _cnt, _cnt);

	if (!lpSceneMng.GameClearFlag())
	{
		lpSceneMng.AddDrawQue({ _gameOver,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });
	}
	else
	{
		lpSceneMng.AddDrawQue({ _gameClear,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });

	}

	_cnt += 2;

	return std::move(own);
}

void ResultScene::Init(void)
{
	_input = std::make_unique<PadState>(DX_INPUT_PAD1);
}

void ResultScene::DrawInit(void)
{
	_gameClear = LoadGraph("image/gameclear.png");
	_gameOver = LoadGraph("image/gameover.png");
}
