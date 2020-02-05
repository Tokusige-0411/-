#include <_DebugConOut.h>
#include "FuncIntoPot.h"
#include <object/Pot.h>

bool FuncIntoPot::operator()(ActQueT& actQue, std::vector<sharedObj>& objList)
{
	auto pot = actQue.second._tageObj.lock();

	if ((*pot)._unitID != UNIT_ID::POT)
	{
		AST();
		return false;
	}

	if (actQue.second._unitID == UNIT_ID::PLAYER)
	{
		actQue.second._hp = actQue.second._hpMax;
		actQue.second.state(STATE::THROWN);
		actQue.second._rad = RAD(rand() % 360);
		actQue.second._vel.x = 5.0;
		actQue.second._vel.y = sqrt(15.0 * 15.0 - actQue.second._vel.x * actQue.second._vel.x);
		actQue.second._gravity = 2 * actQue.second._vel.y / 30.0;
		actQue.second._weight = INT_MAX;
		actQue.second._tageObj.reset();
		return true;
	}

	MixData data = { actQue.second._unitID,actQue.second.getType() };

	((Pot&)(*pot)).AddMixList(data);
	((Pot&)(*pot))._inPotFlg = true;

	actQue.second.state(STATE::NORMAL);
	actQue.second._tageObj.reset();
	actQue.second._dead = true;

	return true;
}