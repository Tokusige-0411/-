#include "TitleScene.h"
#include "SceneMng.h"
#include <_DebugConOut.h>
#include <Input/PadState.h>
#include<Input/InputState.h>
#include<Input/INPUT_ID.h>
#include"GameScene.h"
#include"EntryScene.h"
#include "EntrySceneLocal.h"
#include<common\ImageMng.h>

TitleScene::TitleScene()
{
	_padnum = GetJoypadNum();
	_selectnum = SELECTMENU::START;
	_cursorPos.x =  lpSceneMng.ScreenCenter.x - 200.0;
	_cursorPos.y = -60.0;
	T_logoPos.x = lpSceneMng.ScreenCenter.x;
	T_logoPos.y = 0.0;
	_fadecolor = 255;
	_oldDir = DIR::UP;
	_newDir = DIR::UP;
	for (int s = 0; s < 2; s++)
	{
		_skyPos[s].x = (lpSceneMng.ScreenCenter.x + (s * lpSceneMng.ScreenSize.x));
		_skyPos[s].y = 208.0;
	}
	_fallSpeed = 2.0;
	_fadespeed = 5;
	_cnt = 0;
	DrawInit();
	SoundInit();
	Init();

	func = { &TitleScene::TitleNormal };
}


TitleScene::~TitleScene()
{
	//StopSoundMem(lpSoundMng.GetID(SOUND::TITLE_BGM)[0]);
	InitSoundMem();
}

UniqueBase TitleScene::Update(UniqueBase own)
{
	if (!(CheckSoundMem(lpSoundMng.GetID(SOUND::TITLE_BGM)[0])))
	{
		lpSceneMng.AddSoundQue({ lpSoundMng.GetID(SOUND::TITLE_BGM)[0] , 200, lpSceneMng.ScreenCenter.x,  lpSceneMng.ScreenCenter.y });
	}
	(*_input[0]).Update(_objList);

	SetDrawBright(_fadecolor, _fadecolor, _fadecolor);

	if ((_fadecolor <= 0))
	{
		if (_selectnum == SELECTMENU::START)
		{
			lpSceneMng.AddSoundQue({lpSoundMng.GetID(SOUND::MENU_DEC)[0], 200, lpSceneMng.ScreenCenter.x,  lpSceneMng.ScreenCenter.y });
			return std::make_unique<EntrySceneLocal>();
		}
	}

	lpSceneMng.AddDrawQue({ _titleLogo,lpSceneMng.ScreenCenter.x,T_logoPos.y,0.0,1.0,0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
	for (int s = 0; s < 2; s++)
	{
		lpSceneMng.AddDrawQue({ _skyimage[s], _skyPos[s].x,_skyPos[s].y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });
		_skyPos[s].x -= 0.5;
		if ((_skyPos[s].x + lpSceneMng.ScreenCenter.x) <= 0.0)
		{
			_skyPos[s].x = lpSceneMng.ScreenSize.x + lpSceneMng.ScreenCenter.x;
		}
	}
	lpSceneMng.AddDrawQue({ _titleBG,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });

	(this->*func)();

	return std::move(own);
}

void TitleScene::Init(void)
{
	_input[0] = std::make_unique<PadState>(DX_INPUT_PAD1);
}

void TitleScene::TitleSelect(void)
{
	_oldDir = _newDir;
	_cursorPos.x = lpSceneMng.ScreenCenter.x - 200.0;

	lpSceneMng.AddDrawQue({ _selectTex_1,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y + 150.0,0.0,1.0,0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
	lpSceneMng.AddDrawQue({ _cursor,_cursorPos.x,_cursorPos.y,0.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });

	(*_input[0]).Update(_objList);


	if (((*_input[0]).LStickState().first.isInput))
	{
		if ((*_input[0]).LStickState().first.dir == DIR::DOWN)
		{
			_newDir = DIR::DOWN;
			if ((_selectnum == SELECTMENU::START) && (_newDir != _oldDir))
			{
				_selectnum = SELECTMENU::END;
			}
			else if ((_selectnum == SELECTMENU::END) && (_newDir != _oldDir))
			{
				_selectnum = SELECTMENU::START;
			}
		}
		else if ((*_input[0]).LStickState().first.dir == DIR::UP)
		{
			_newDir = DIR::UP;
			if ((_selectnum == SELECTMENU::END) && (_newDir != _oldDir))
			{
				_selectnum = SELECTMENU::START;
			}
		else if ((_selectnum == SELECTMENU::START) && (_newDir != _oldDir))
			{
				_selectnum = SELECTMENU::END;
			}
		}
		else
		{
			DIR::LEFT;
			_newDir = DIR::LEFT;
		}
	}
	else
	{
		DIR::LEFT;
		_newDir = DIR::LEFT;
	}


	if (_selectnum == SELECTMENU::START)
	{
		_cursorPos.y = lpSceneMng.ScreenCenter.y + 115.0;
	}
	else
	{
		_cursorPos.y = lpSceneMng.ScreenCenter.y + 185.0;
	}
	if (((*_input[0]).btnState(INPUT_ID::BTN_B).first))
	{
		if (_selectnum == SELECTMENU::END)
		{
			func = &TitleScene::TitleEnd;
		}
		else
		{
			func = &TitleScene::LAST;
		}
	}
	if (((*_input[0]).btnState(INPUT_ID::BTN_A).first))
	{
		func = &TitleScene::TitleNormal;
		TRACE("タイトルセレクト終了\n");
	}
}
void TitleScene::TitleEnd(void)
{
	 lpSceneMng.SetGameEndFlag(true);
}

void TitleScene::TitleNormal(void)
{
	if (T_logoPos.y < lpSceneMng.ScreenCenter.y - 100.0)
	{
		T_logoPos.y += _fallSpeed;
	}
	else
	{
		T_logoPos.y = (lpSceneMng.ScreenCenter.y - 100.0);
	}
	if (((*_input[0]).btnState(INPUT_ID::BTN_START).first))
	{
		_cnt = 0;

		T_logoPos.y = (lpSceneMng.ScreenCenter.y - 100.0);
		_selectnum = SELECTMENU::START;
		func = &TitleScene::TitleSelect;
		TRACE("タイトルセレクト開始\n");
	}
	else
	{
		if ((_cnt / 45) % 2 == 0)
		{
			lpSceneMng.AddDrawQue({ _titleTex,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y + 200.0,0.0,1.0,0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
		}

		_cnt++;
	}
}

void TitleScene::LAST(void)
{
	_fadecolor -= _fadespeed;
}

void TitleScene::DrawInit(void)
{
	//ImageMng::GetInstance().GetID( "TitleBG", "image/TitleBG.png");
	_titleBG   = LoadGraph("image/TitleBG.png");
	for (int s = 0; s < 2; s++)
	{
		_skyimage[s] = LoadGraph("image/sky.png");
	}
	_cursor    = LoadGraph("image/selector.png");
	_selectTex_1 = LoadGraph("image/TitleSelect_1.png");
	_titleTex  = LoadGraph("image/start_button.png");
	_titleLogo = LoadGraph("image/TitleLogo.png");
}

void TitleScene::SoundInit(void)
{
	lpSoundMng.GetID(SOUND::TITLE_BGM, "sound/Title_bgm.mp3");
	lpSoundMng.GetID(SOUND::MENU_DEC, "sound/Menu_dec.mp3");
}
