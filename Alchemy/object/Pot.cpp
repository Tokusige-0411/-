#include <DxLib.h>
#include "Pot.h"
#include <_DebugConOut.h>
#include <_DebugDispOut.h>
#include <EffekseerForDXLib.h>
#include "FOLLOWER_TYPE.h"
#include "ENEMY_TYPE.h"
#include "func/PotNormal.h"
#include "func/PotHold.h"
#include "func/PotThrow.h"
#include <EffectMng.h>
#include "../Map.h"
#include <UI_Mng.h>

Pot::Pot()
{
	Init();
}


Pot::~Pot()
{
	_combiList.clear();
}

void Pot::Update(std::vector<sharedObj>& objList)
{
	try
	{
		_funcState[state()](*this, objList);
	}
	catch (...)
	{
		AST();
	}

	if (_hp<=0)
	{
		resultFlg(true);
		lpSceneMng.SetGameClearFlag(false);
		return;
	}

	lpMap.ChangeChip(_pos, _rad, 3);

	if (_inPotFlg)
	{
		// 再生ﾁｪｯｸ
		if (IsEffekseer2DEffectPlaying(_effectID) == -1)
		{
			if (_effectFlg)
			{
				StopEffekseer2DEffect(_effectID);
				_effectFlg = false;
				_inPotFlg = false;
				return;
			}
		}
		else
		{
			_effectFlg = true;
		}
		// ｴﾌｪｸﾄをｷｭｰに投げる
		lpEffectMng.AddEffectQue({ *this,_pos - Vector2Dbl{0.0,static_cast<double>(_size.y / 2 - 10)},0,EFFECT::LANDING,_zOrder + 1 });
	}
}

void Pot::AddMixList(MixData data)
{
	UNIT_ID id;
	int type;
	std::tie(id, type) = data;

	lpSceneMng.AddSoundQue({ lpSoundMng.GetID(SOUND::COMBO)[_comboNum], 255, _pos.x, _pos.y });

	if (id == UNIT_ID::ITEM) 
	{
		_comboNum++;
		// コンボの回数によって加える数を増やしてみる
		/*for (int i = 0; i < _comboNum; i++)
		{
			_mixList.emplace_back(data);
		}*/
		_mixList.emplace_back(data);
		state(STATE::HOLD);
	}
	else if (id == UNIT_ID::FOLLOWER)
	{
		_followerCnt[static_cast<int>(log2(type))]++;
		Text mes;
		mes.MakeText("つぼにスタックされました", -8);
		lpUI_Mng.AddMessage(mes);
		mes.MakeText("　ボタンでかくにんできます", -8);
		mes.AddIcon(TEXT_ICON::X, { 15,16 }, 2.0);
		lpUI_Mng.AddMessage(mes);
	}

	
	_comboCnt = COMBO_TIME_MAX - 30 * (_comboNum-1);
}

std::vector<int>& Pot::followerCnt(void)
{
	return _followerCnt;
}


void Pot::Init(void)
{
	// ----------アニメーション登録開始
	AnimVector data;
	ImageKey key = { IMG::POT,STATE::NORMAL };

	data.emplace_back(IMAGE_ID(key)[0], 30);
	data.emplace_back(IMAGE_ID(key)[0], 60);

	SetAnim({ STATE::NORMAL,DIR::DOWN }, data);
	
	data.emplace_back(IMAGE_ID(key)[0], 30);
	data.emplace_back(IMAGE_ID(key)[0], 60);
	SetAnim({ STATE::THROW,DIR::DOWN }, data);

	key = { IMG::POT,STATE::HOLD };;

	data.emplace_back(IMAGE_ID(key)[0], 5);
	data.emplace_back(IMAGE_ID(key)[1], 10);
	data.emplace_back(IMAGE_ID(key)[2], 15);
	data.emplace_back(IMAGE_ID(key)[3], 20);
	data.emplace_back(IMAGE_ID(key)[4], 20);

	SetAnim({ STATE::HOLD,DIR::DOWN }, data);
	_unitID = UNIT_ID::POT;
	_team = TEAM_TAG::ALLY_TEAM;
	_searchRange = 40.0;
	_weight = INT_MAX;
	_pos = lpSceneMng.WorldCenter;
	_size = { 64,84 };
	_glowID = MakeScreen(_size.x * 2, _size.y * 2, true);
	_speed = 0.0;
	_comboCnt = COMBO_TIME_MAX;
	_hpMax = 200;
	_hp = _hpMax;
	_inPotFlg = false;
	_effectScreen = MakeScreen(static_cast<int>(lpEffectMng._screenSize.x), static_cast<int>(lpEffectMng._screenSize.y), true);

	_followerCnt.resize(6);
	for (int i = 0; i < 6; i++)
	{
		_followerCnt[i] = 0;
	}

	_gaugeImg[0] = lpImageMng.GetID({ IMG::COMBO_GAUGE,STATE::HOLD }, "image/comboGaugeFrame.png")[0];
	_gaugeImg[1] = lpImageMng.GetID({ IMG::COMBO_GAUGE,STATE::NORMAL }, "image/comboGauge.png")[0];
	_gaugeID = MakeScreen(90, 26, true);

	// AddInstanceを通らずにインスタンスされるので通し番号を足しておく
	_serialNum = lpSceneMng.serialNumCnt();
	lpSceneMng.AddSerialNum();

	_funcState = { {STATE::NORMAL,PotNormal()},{STATE::HOLD,PotHold()},{STATE::THROW,PotThrow()} };

	CombiListInit();
	// 初期アニメーション
	dir(DIR::DOWN);
	state(STATE::NORMAL);
}

void Pot::CombiListInit(void)
{
	FILE* filePt;
	fopen_s(&filePt, "data/mixData.csv", "rb");

	if (filePt == nullptr)
	{
		AST();
		return;
	}

	int confType = 0;
	int prob[6] = {0,0,0,0,0,0};
	int probCnt = 0;
	MixCombination combi;

	while (fscanf_s(filePt, "%d, %d, %d, %d, %d, %d, %d", &confType, &prob[0], &prob[1], &prob[2], &prob[3], &prob[4], &prob[5]) != EOF)
	{
		int probCnt = 0;
		probCnt = 0;
		combi.confType = confType;
		for (int i = 0; i < 6; i++)
		{
			while (prob[i])
			{
				combi.mixProb[probCnt] = i;
				probCnt++;
				prob[i]--;
			}
		}
		_combiList.emplace_back(combi);
	}

	fclose(filePt);
}

void Pot::InitFunc(void)
{
}
