#include <DxLib.h>
#include "Item.h"
#include "../_debug/_DebugConOut.h"
#include "../common/ImageMng.h"
#include "func/ItemNormal.h"
#include "func/HoldenUpdate.h"
#include "func/ThrownUpdate.h"
#include "../scene/func/FuncThrow.h"
#include <EffectMng.h>

Item::Item(int type, Vector2Dbl pos)
{
	_pos = pos;
	_type = static_cast<FOLLOWER_TYPE>(type);
	Init();
}

Item::~Item()
{
}

void Item::Update(std::vector<sharedObj>& objList)
{
	if (DestroyProc())
	{
		return;
	}
	try
	{
		_funcState[state()](*this, objList);
		_floatCnt++;
	}
	catch (...)
	{
		AST();
	}

	if (state() == STATE::NORMAL)
	{
		_zOrder = 0;
	}
	else
	{
		_zOrder = static_cast<int>(_height);
	}
}

int Item::getType(void)
{
	return static_cast<int>(_type);
}

int Item::floatCnt(void)
{
	return _floatCnt;
}

void Item::Init(void)
{
	// ----------アニメーション登録開始
	_unitID = UNIT_ID::ITEM;

	_effectScreen = MakeScreen(static_cast<int>(lpEffectMng._screenSize.x), static_cast<int>(lpEffectMng._screenSize.y), true);

	AnimVector data;
	ImageKey key;

	switch (_type)
	{
	case FOLLOWER_TYPE::OCTOPUS:
		key = { IMG::ITEM_SLIME,STATE::NORMAL };
		_size = { 48,48 };
		break;
	case FOLLOWER_TYPE::BEE:
		key = { IMG::ITEM_BAT,STATE::NORMAL };
		_size = { 48,48 };
		break;
	case FOLLOWER_TYPE::GHOST:
		key = { IMG::ITEM_GHOST,STATE::NORMAL };
		_size = { 48,48 };
		break;
	case FOLLOWER_TYPE::MUSH:
		key = { IMG::ITEM_MUSH,STATE::NORMAL };
		_size = { 48,48 };
		break;
	case FOLLOWER_TYPE::DAEMON:
		key = { IMG::ITEM_DEMON,STATE::NORMAL };
		_size = { 48,48 };
		break;
	case FOLLOWER_TYPE::POT:
		key = { IMG::ITEM_POT,STATE::NORMAL };
		_size = { 40,40 };
		break;
	case FOLLOWER_TYPE::MAX:
		AST();
		break;
	default:
		AST();
		break;
	}

	int color = rand() % 3;
	auto dataEmplace = [&](FOLLOWER_TYPE _type) {
		if (_type != FOLLOWER_TYPE::POT)
		{
			data.emplace_back(IMAGE_ID(key)[0], 10);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				data.emplace_back(IMAGE_ID(key)[i + color * 4], 10 + (10 * i));
			}
		}
	};

	// -- 通常
	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		dataEmplace(_type);
		SetAnim({ STATE::NORMAL,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		dataEmplace(_type);
		SetAnim({ STATE::HOLDEN,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		dataEmplace(_type);
		SetAnim({ STATE::THROWN,dir }, data);
	}
	// 初期アニメーション
	state(STATE::THROWN);
	_vel.y = 5.0;
	_gravity = 0.2;

	dir(DIR::DOWN);
	_floatCnt = 0;
	_glowID = MakeScreen(_size.x * 2, _size.y * 2, true);
	_funcState = { {STATE::NORMAL,ItemNormal()},{ STATE::HOLDEN,HoldenUpdate() },{ STATE::THROWN,ThrownUpdate() } };
}
