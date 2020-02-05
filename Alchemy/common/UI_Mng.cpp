#include <DxLib.h>
#include <scene/SceneMng.h>
#include "UI_Mng.h"
#include <object/Player.h>
#include <Camera.h>

std::unique_ptr<UI_Mng, UI_Mng::UI_MngDeleter> UI_Mng::_sInstance(new UI_Mng());

void UI_Mng::AddMessage(Text mes)
{
	(*_mesBoard).AddMesList(mes);
	_fadeFlag = false;
}

void UI_Mng::AddUIQue(UIQue que)
{
	_uiList.emplace_back(que);
}

void UI_Mng::Draw(void)
{
	SetDrawScreen(_screen);
	ClsDrawScreen();

	if (!_fadeCnt)
	{
		if (!lpCamera.exMoveFlag())
		{
			if (Player::GetpCnt() > 1)
			{
				DrawRotaGraph(640, 360, 1.0, 0.0, _barImg, true);
			}
		}

		for (auto ui : _uiList)
		{
			double x, y;
			int image;

			std::tie(x, y, image) = ui;

			DrawGraph(static_cast<int>(x), static_cast<int>(y), image, true);
		}
		DrawRotaGraph(1240, 40, 2.0, 0.0, _btnGuideImg, true);
		(*_mesBoard).DrawUpdate();

		if (Player::GetpCnt() <= 1)
		{
			DrawGraph(0, 400, (*_mesBoard).mesScreen(), true);
		}

		_fadeCnt = 0;
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255 - std::abs(255 - _fadeCnt));
		DrawGraph(0, 0, _blackImg, true);
		_fadeCnt -= 3;
	}

	// ƒJƒƒ‰‚ð“®‚©‚µ‚Ä‚¢‚¢‚©
	_fadeFlag = _fadeCnt <= 255 ? false : true;

	_uiList.clear();
	SetDrawScreen(DX_SCREEN_BACK);
}

void UI_Mng::UI_Init(void)
{
	_screen = MakeScreen(static_cast<int>(lpSceneMng.ScreenSize.x), static_cast<int>(lpSceneMng.ScreenSize.y), true);
	_mesBoard = std::make_unique<Message>();
	_btnGuideImg = LoadGraph("image/buttonGuide.png", true);
	_barImg = LoadGraph("image/bar.png",true);
	_blackImg = LoadGraph("image/black.png", true);
}

void UI_Mng::UI_ReSet(void)
{
	SetDrawScreen(_screen);
	ClsDrawScreen();
	SetDrawScreen(DX_SCREEN_BACK);
}

int UI_Mng::FadeCnt(void)
{
	return _fadeCnt;
}

void UI_Mng::FadeCnt(int cnt)
{
	if (_fadeCnt <= 0)
	{
		_fadeCnt = cnt;
	}
}

bool UI_Mng::FadeFlag(void)
{
	_fadeFlag = (_fadeCnt <= 255 ? false : true);
	return _fadeFlag;
}

int UI_Mng::screen(void)
{
	return _screen;
}

UI_Mng::UI_Mng()
{
}


UI_Mng::~UI_Mng()
{
}
