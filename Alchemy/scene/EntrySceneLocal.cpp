#include "EntrySceneLocal.h"
#include "SceneMng.h"
#include <_DebugConOut.h>
#include <Input/PadState.h>
#include<Input/InputState.h>
#include<Input/INPUT_ID.h>
#include"GameScene.h"
#include<object/PlNum.h>


EntrySceneLocal::EntrySceneLocal()
{
	_padnum = GetJoypadNum();
	_entryPadnum = 0;
	_fadespeed = 15;
	_fadecolor = 0;
	for (int i = 0; i < 2; i++)
	{
		_entrnum[i] = 0;
		_cnt[i] = 0;
		_plNum[i] = PlNum::PL_NON;
		_playerScreen[i] = MakeScreen(320, 300, true);
		_readyFlag[i] = false;
	}
	DrawInit();
	Init();

	func = { &EntrySceneLocal::EntryStart };
}


EntrySceneLocal::~EntrySceneLocal()
{
}

UniqueBase EntrySceneLocal::Update(UniqueBase own)
{
	SetDrawBright(_fadecolor, _fadecolor, _fadecolor);

	for (int i = 0; i < _padnum; i++)
	{
		(*_input[i]).Update(_objList);
		if (_readyFlag[0] && _readyFlag[1])
		{
			if (_cnt[i] / 30 % 2)
			{
				lpSceneMng.AddDrawQue({ _start,(320.0 + static_cast<double>((i *  640.0))),370 ,20 * (3.141592 / 180),1.0,0.0,INT_MAX,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
			}
			if ((*_input[i]).btnState(INPUT_ID::BTN_START).first)
			{
				lpSceneMng.AddPlayerQue({ _plNum[0], {1080.0,800.0 } ,DX_INPUT_PAD1 });
				lpSceneMng.AddPlayerQue({ _plNum[1], {1280.0,800.0 } ,DX_INPUT_PAD2 });
				return std::make_unique<GameScene>();
			}
		}

		if (_readyFlag[0])
		{
			if (_cnt[i] / 30 % 2)
			{
				lpSceneMng.AddDrawQue({ _start,320.0,370 ,20 * (3.141592 / 180),1.0,0.0,INT_MAX,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
			}
			if ((*_input[0]).btnState(INPUT_ID::BTN_START).first&&_plNum[1] == PlNum::PL_NON)
			{
				lpSceneMng.AddPlayerQue({ _plNum[0], {1080.0,800.0 } ,DX_INPUT_PAD1 });
				return std::make_unique<GameScene>();
			}
		}

		if (_padnum == 1 && _readyFlag[0])
		{
			if (_cnt[i] / 30 % 2)
			{
				lpSceneMng.AddDrawQue({ _start,(320.0 + static_cast<double>((i *  640.0))),370 ,20 * (3.141592 / 180),1.0,0.0,INT_MAX,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
			}
			if ((*_input[0]).btnState(INPUT_ID::BTN_START).first)
			{
				lpSceneMng.AddPlayerQue({ _plNum[0], {1080.0,800.0 } ,DX_INPUT_PAD1 });
				return std::make_unique<GameScene>();
			}
		}
	}
	lpSceneMng.AddDrawQue({ _skyimage, lpSceneMng.ScreenCenter.x,208.0,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });
	lpSceneMng.AddDrawQue({ _entryBG,lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,1.0,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });

	(this->*func)();

	return std::move(own);
}

void EntrySceneLocal::Init(void)
{
	_input[0] = std::make_unique<PadState>(DX_INPUT_PAD1);
	_input[1] = std::make_unique<PadState>(DX_INPUT_PAD2);
}

void EntrySceneLocal::EntryStart(void)
{
	if (_fadecolor < 255)
	{
		_fadecolor += _fadespeed;
	}
	else
	{
		func = &EntrySceneLocal::EntryNormal;
	}
}

void EntrySceneLocal::EntryNormal(void)
{
	for (int i = 0; i < _padnum; i++)
	{
		_cnt[i]++;
		if (_readyFlag[i])
		{
			SetDrawBright(255, 255, 255);
		}
		else
		{
			SetDrawBright(200, 200, 200);
		}
		SetDrawScreen(_playerScreen[i]);
		ClsDrawScreen();
		if (_plNum[i] != PlNum::PL_NON)
		{
			if (!_readyFlag[i])
			{
				lpSceneMng.AddDrawQue({ _b,(320.0 + static_cast<double>((i *  640.0))),455.0,0.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
				lpSceneMng.AddDrawQue({ _arrow[0],224 + (i * 640),370,.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
				lpSceneMng.AddDrawQue({ _arrow[1],416 + (i * 640),370,.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
			}
			DrawRotaGraph(160, 150, 1.0, 0, _entryImage[static_cast<size_t>(_plNum[i])][((_cnt[i] / 10) % 4)], true);
		}
		lpSceneMng.AddDrawQue({ _playerScreen[i],(320.0 + static_cast<double>((i *  640.0))),370.0,0.0,1.0,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
		if ((*_input[i]).btnState(INPUT_ID::BTN_START).first&&_plNum[i] == PlNum::PL_NON)
		{
			_plNum[i] = static_cast<PlNum>(i);
		}
		SetDrawScreen(DX_SCREEN_BACK);
		if (_plNum[i] == PlNum::PL_NON)
		{
			if (_cnt[i] / 30 % 2)
			{
				lpSceneMng.AddDrawQue({ _entry,(320.0 + static_cast<double>((i *  640.0))),370 ,-20*(3.141592 / 180),1.8,0.0,0,LAYER::CHAR,DX_BLENDMODE_NOBLEND,255 });
			}
			continue;
		}

		if ((*_input[i]).btnState(INPUT_ID::BTN_B).first && !(*_input[i]).btnState(INPUT_ID::BTN_B).second&&_plNum[0] != _plNum[1])
		{
			_readyFlag[i] = true;
		}
		if ((*_input[i]).btnState(INPUT_ID::BTN_A).first && !(*_input[i]).btnState(INPUT_ID::BTN_A).second)
		{
			_readyFlag[i] = false;
		}

		if (_readyFlag[i])
		{
			continue;
		}

		if ((*_input[i]).btnState(INPUT_ID::RIGHT).first && !(*_input[i]).btnState(INPUT_ID::RIGHT).second || (*_input[i]).LStickState().first.dir == DIR::RIGHT && (*_input[i]).LStickState().second.dir != DIR::RIGHT)
		{
			
			if (_plNum[i] < PlNum::PL_04)
			{
				++_plNum[i];
			}
			else
			{
				_plNum[i] = PlNum::PL_01;
			}
			if (_plNum[0] == _plNum[1] && _plNum[i] != PlNum::PL_04)
			{
				++_plNum[i];
			}
			else if(_plNum[0] == _plNum[1])
			{
				_plNum[i] = PlNum::PL_01;
			}
			else
			{
				// ‰½‚à‚µ‚È‚¢
			}

		}
		if ((*_input[i]).btnState(INPUT_ID::LEFT).first && !(*_input[i]).btnState(INPUT_ID::LEFT).second || (*_input[i]).LStickState().first.dir == DIR::LEFT && (*_input[i]).LStickState().second.dir != DIR::LEFT)
		{
			if (_plNum[i] > PlNum::PL_01)
			{
				--_plNum[i];
			}
			else
			{
				_plNum[i] = PlNum::PL_04;
			}
			if (_plNum[0] == _plNum[1] && _plNum[i] != PlNum::PL_01)
			{
				--_plNum[i];
			}
			else if (_plNum[0] == _plNum[1])
			{
				_plNum[i] = PlNum::PL_04;
			}
			else
			{
				// ‰½‚à‚µ‚È‚¢
			}
		}
	}
	SetDrawBright(255, 255, 255);
}

void EntrySceneLocal::DrawInit(void)
{
	_entryBG = LoadGraph("image/EntryBG.png");
	LoadDivGraph("image/Player_walk.png", 16, 4, 4, 266, 249, *_entryImage);
	LoadDivGraph("image/arrow.png", 2, 2, 1, 64, 64, _arrow);
	_skyimage = LoadGraph("image/sky.png");
	_b = LoadGraph("image/b.png");
	_a = LoadGraph("image/a.png");
	_start = LoadGraph("image/start_button.png");
	_entry = LoadGraph("image/entry.png");
}

