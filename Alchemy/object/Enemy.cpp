#include <algorithm>
#include <_DebugConOut.h>
#include "Enemy.h"
#include <Input/PadState.h>
#include <Input/AI/enemyAI.h>
#include "Enemy_Init\SlimeInit.h"
#include "Enemy_Init\BatInit.h"
#include "Enemy_Init\GhostInit.h"
#include "Enemy_Init\MushroomInit.h"
#include "Enemy_Init\DemonInit.h"
#include <EffectMng.h>
#include "../Map.h"


std::map<ENEMY_TYPE, std::function<bool(Obj&)>> Enemy::enemyInitMap = { {ENEMY_TYPE::SLIME,SlimeInit()},{ ENEMY_TYPE::BAD,BatInit() },{ ENEMY_TYPE::MUSH,MushroomInit() },{ ENEMY_TYPE::DAEMON,DemonInit() },{ ENEMY_TYPE::GHOST,GhostInit() } };

Enemy::Enemy()
{
	Init();
}

Enemy::Enemy(EnemyState enemyState, sharedObj potObj)
{
	std::tie(_enemyType, _pos, _serialNum) = enemyState;
	_potObj = potObj;
	Init();
}

void Enemy::Update(std::vector<sharedObj>& objList)
{
	if (DestroyProc())
	{
		return;
	}

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

	try
	{
		_funcState[state()](*this,objList);
	}
	catch (...)
	{
		AST();
	}

	lpMap.ChangeChip(_pos, _rad, 0);

	// ���S
	if (_hp <= 0)
	{
		alive(false);
	}

	_zOrder = static_cast<int>(_height);
}

const ENEMY_TYPE Enemy::enemyType(void) const
{
	return _enemyType;
}

int Enemy::getType(void)
{
	return static_cast<int>(_enemyType);
}

Enemy::~Enemy()
{
	lpSceneMng.AddInstanceQue({ UNIT_ID::ITEM,static_cast<int>(_enemyType),_pos,0 });
}

void Enemy::Init(void)
{
	try
	{
		enemyInitMap[_enemyType](*this);
	}
	catch (...)
	{
		AST();
	}

	// ----------�A�j���[�V�����o�^�J�n
	AnimVector data;
	ImageKey death = { IMG::BLAST,STATE::DEATH };

	// ���񂾂Ƃ�
	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		for (int i = 0; i < 10; i++)
		{
			data.emplace_back(IMAGE_ID(death)[i], i * 3);
		}
		data.emplace_back(-1, 33);
		SetAnim({ STATE::DEATH,dir }, data);
	}
	
	_hp = _hpMax;
	_coolCntMax = 100;
	_coolCnt = _coolCntMax;
	_weight = INT_MAX;
	_glowID = MakeScreen(_size.x * 2, _size.y * 2, true);
	_effectScreen = MakeScreen(static_cast<int>(lpEffectMng._screenSize.x), static_cast<int>(lpEffectMng._screenSize.y), true);
	_unitID = UNIT_ID::ENEMY;
	_team = TEAM_TAG::ENEMY_TEAM;

	// �����A�j���[�V����
	state(STATE::NORMAL);
	dir(DIR::DOWN);
}


