#include "FuncHold.h"
#include <object/Player.h>
#include <_DebugConOut.h>

bool FuncHold::operator()(ActQueT & actQue, std::vector<sharedObj>& objList)
{
	if (actQue.second.tageObj().expired())
	{
		return false;
	}

	for (auto obj : objList)
	{
		if ((*obj).unitID() == UNIT_ID::PLAYER)
		{
			if (((Player&)(*obj))._plNum == ((Player&)actQue.second)._plNum)
			{
				//�Ⓤ���׸ނ����Z�b�g���邩
				//((Player&)actQue.second)._throwPot = true;

				auto holdenObj = ((Player&)actQue.second)._tageObj.lock(); // ���I�u�W�F

				(*holdenObj)._holdPlayer = obj;
				(*holdenObj)._zOrder = INT_MAX;

				(*holdenObj)._height = static_cast<double>((actQue.second)._size.y / 2);

				for (auto holdObj : ((Player&)actQue.second)._holdList)
				{
					(*holdenObj)._height += static_cast<double>((*holdObj)._size.y);
				}
				(*holdenObj).state(STATE::HOLDEN);
				((Player&)actQue.second)._holdList.emplace_back(holdenObj);

				(*holdenObj)._tageObj.reset();
				actQue.second.state(STATE::HOLD);

				return true;
			}
		}
	}
	return false;
}
