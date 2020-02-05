#include "FollowPotInit.h"
#include "../../Input/Follower_AI/FollowerPot.h"
#include "../func/TestEnemyUpdate.h"
#include "../func/HoldenUpdate.h"
#include "../func/ThrownUpdate.h"
#include "../func/PotAttack.h"

bool FollowPotInit::operator()(Obj & obj)
{
	// ----------アニメーション登録開始
	AnimVector data;

	ImageKey key = { IMG::FOL_POT,STATE::NORMAL };

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 15);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 45);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 59);
		obj.SetAnim({ STATE::NORMAL,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 15);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 ], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 ], 45);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 ], 60);
		obj.SetAnim({ STATE::HOLDEN,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 15);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 45);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 59);
		obj.SetAnim({ STATE::THROWN,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 6);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 12);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 18);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 24);
		obj.SetAnim({ STATE::ATTACK,dir }, data);
	}

	//_input = std::make_unique<PadState>(DX_INPUT_PAD1);
	obj._input = std::make_unique<FollowerPot>(obj);

	obj._searchRange = 150.0;
	obj._attackRange = 40.0;

	obj._size = { 32,32 };

	obj._speed = 0.5;
	obj._hpMax = 6;
	obj._coolCntMax = 30;
	obj._power = 1;

	obj._funcState = { { STATE::NORMAL,TestEnemyUpdate() },{ STATE::HOLDEN,HoldenUpdate() },{ STATE::THROWN,ThrownUpdate() },{ STATE::ATTACK,PotAttack() } };


	return true;
}
