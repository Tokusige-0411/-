#include <Dxlib.h>
#include <_DebugConOut.h>
#include <EffekseerForDXLib.h>
#include <EffectMng.h>
#include <Camera.h>
#include "../Map.h"
#include "Boss.h"
#include <input/InputState.h>
#include <Input/Enemy_AI/Boss_AI.h>
#include "func/TestEnemyUpdate.h"
#include "func/BossAttack.h"
#include "Player.h"
#include <UI_Mng.h>

Boss::Boss()
{
	Init();
}

Boss::Boss(sharedObj potObj)
{
	_potObj = potObj;
	Init();
}

Boss::~Boss()
{
}

void Boss::Update(std::vector<sharedObj>& objList)
{
	if (DestroyProc())
	{
		return;
	}

	// ---- 出現中
	if (lpCamera.exMoveFlag())
	{
		AddBoss();
		return;
	}


	if (_hp <= 0)
	{
		if (state() != STATE::NORMAL)
		{
			StopEffekseer2DEffect(_effectID);
			_effectFlg = false;
			state(STATE::NORMAL);
		}
		BossDeath();
		return;
	}
	else
	{
		// 死亡
		if (state() == STATE::NORMAL)
		{
			if (lpSceneMng.frameCnt() % 2)
			{
				(*_input).SetOld();
				(*_input).StateReset();

				(*_input).Update(objList);
				if ((*_input).btnState(INPUT_ID::BTN_B).first)
				{
					_effectFlg = false;
					state(STATE::ATTACK);
				}
			}
		}
	}

	try
	{
		_funcState[state()](*this, objList);
	}
	catch (...)
	{
		AST();
	}

	lpMap.ChangeChip(_pos, _rad, 0);

	_zOrder = static_cast<int>(_height);
}

void Boss::Init()
{
	AnimVector data;

	ImageKey key = { IMG::BOSS, STATE::NORMAL };

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 10);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 20);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 40);
		SetAnim({ STATE::NORMAL, dir }, data);
	}

	// 攻撃用ダミー画像
	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 10);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 20);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 40);
		SetAnim({ STATE::ATTACK,dir }, data);
	}

	// 死んだとき
	key = { IMG::BLAST, STATE::DEATH };
	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		for (int i = 0; i < 10; i++)
		{
			data.emplace_back(IMAGE_ID(key)[i], i * 3);
		}
		data.emplace_back(-1, 33);
		SetAnim({ STATE::DEATH,dir }, data);
	}

	_attackRange = 100.0;
	_searchRange = 200.0;
	_speed = 0.5;
	_pos = { lpSceneMng.WorldSize.x / 2.0,lpSceneMng.WorldSize.y / 6.0 };
	_power = 15;

	// HP
	_hpMax = 200;
	_hp = 1;

	_team = TEAM_TAG::ENEMY_TEAM;
	_unitID = UNIT_ID::BOSS;

	_coolCntMax = 100;
	_coolCnt = _coolCntMax;
	_input = std::make_unique<Boss_AI>(*this);
	_extRate = 3.0;
	_size = Vector2Int{ 94,100 }*static_cast<int>(_extRate);
	_deathCnt = 0;

	_weight = INT_MAX;
	_glowID = MakeScreen(_size.x * 2, _size.y * 2, true);
	_effectScreen = MakeScreen(static_cast<int>(lpEffectMng._screenSize.x), static_cast<int>(lpEffectMng._screenSize.y), true);
	deathScreen = MakeScreen(static_cast<int>(lpSceneMng.WorldSize.x*2), static_cast<int>(lpSceneMng.WorldSize.y*2), true);
	_bossHPImg[0] = lpImageMng.GetID({ IMG::BOSS_HP_G,STATE::NORMAL }, "image/bossHP_Frame.png")[0];
	_bossHPImg[1] = lpImageMng.GetID({ IMG::BOSS_HP_G,STATE::HOLD }, "image/HP_bar.png")[0];
	_bossHPImg[2] = lpImageMng.GetID({ IMG::BOSS_HP_G,STATE::HOLDEN }, "image/bossHP_Back.png")[0];

	_funcState = { {STATE::NORMAL, TestEnemyUpdate()}, {STATE::ATTACK, BossAttack()} };

	_hpID = MakeScreen(1280, 16, true);

	// 出現時必要なもの
	_height = 5000.0;
	_addVec = (_height) / HEIGHT_CNT_MAX;
	_heightCnt = 0;
	_addFlag = false;
	_bossHPCount = 1;
	_addCount = ADD_CNT_MAX;

	// 初期アニメーション
	state(STATE::NORMAL);
	dir(DIR::DOWN);
}

bool Boss::AddBossMove()
{
	_height -= _addVec;

	if (_heightCnt >= HEIGHT_CNT_MAX)
	{
		_height = -20;
		return true;
	}	
	lpSceneMng.SetShakeCount(10);	// 画面を揺らすよ
	_heightCnt++;
	return false;
}

bool Boss::AddBossHP(void)
{
	_hp += 1;
	_bossHPCount++;
	if (_hp >= _hpMax)
	{
		_bossHPCount = 0;
		_hp = _hpMax;
		return true;
	}
	return false;
}

void Boss::DrawHP(void)
{
		SetDrawScreen(_hpID);
		ClsDrawScreen();

		DrawGraph(0, 0, _bossHPImg[2], true);

		if (_bossHPCount % 2 == 0)		// 出現中はちかちかさせるよ
		{
			DrawRectGraph(0, 0, 0, 0, 1280 * _hp / _hpMax, 16, _bossHPImg[1], true, false);
		}

		DrawGraph(0, 0, _bossHPImg[0], true);

		SetDrawScreen(DX_SCREEN_BACK);

		lpUI_Mng.AddUIQue({
			0.0,
			0.0,
			_hpID
			});
}

void Boss::BossDeath(void)
{
	_effectScreen = deathScreen;
	// 再生ﾁｪｯｸ
	if (IsEffekseer2DEffectPlaying(_effectID) == 0)
	{
		_effectFlg = true;
	}

	if (_deathCnt > 350)
	{
		StopEffekseer2DEffect(_effectID);
		_effectFlg = false;
		alive(false);
		SetresultFlg(true);
		lpSceneMng.SetGameClearFlag(true);
		return;
	}

	if (_effectFlg)
	{
		_deathCnt++;
	}
	// ｴﾌｪｸﾄをｷｭｰに投げる
	lpEffectMng.AddEffectQue({ *this,_pos,0,EFFECT::DEATH,INT_MAX });
}

void Boss::AddBoss(void)
{
	if (AddBossMove())								// 上から降ってきてる
	{
		if (AddBossHP())							// HP演出
		{
			// 再生ﾁｪｯｸ
			if (IsEffekseer2DEffectPlaying(_effectID) == -1)
			{
				if (_effectFlg)
				{
					if (lpCamera.MoveData())
					{
						StopEffekseer2DEffect(_effectID);
						//lpUI_Mng.FadeCnt(510);
						_effectFlg = false;
						lpCamera.exMoveFlag(false);		// 終了
						return;
					}
				}
			}
			else
			{
				_effectFlg = true;
			}
			// ｴﾌｪｸﾄをｷｭｰに投げる
			lpEffectMng.AddEffectQue({ *this,_pos,0,EFFECT::DARK,_zOrder + 1 });
		}
	}
}
