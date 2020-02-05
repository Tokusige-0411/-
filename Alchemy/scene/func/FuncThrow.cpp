#include "FuncThrow.h"
#include <object/Player.h>
#include <object/Pot.h>
#include <_DebugConOut.h>

bool FuncThrow::operator()(ActQueT& actQue, std::vector<sharedObj>& objList)
{
	if (actQue.second.unitID() == UNIT_ID::PLAYER)
	{
		// �v���C���[��������Ƃ�
		Player& player = ((Player&)actQue.second);
		auto thrownObj = player._holdList.front();
		if (!(player._tageObj.expired()))
		{
			// ����^�[�Q�b�g�ɂ��Ă���Ƃ��͚�Ɍ������ē�����
			if ((*player._tageObj.lock()).unitID() == UNIT_ID::POT)
			{
				(*thrownObj)._rad = player._rad;
				player._tageObj.reset();

				for (auto obj : objList)
				{
					if ((*obj).unitID() == UNIT_ID::POT)
					{
						(*thrownObj).SetTageObj(obj);

						// ���������̑�����ݒ�
						(*thrownObj)._vel.x = sqrt(LengthSquare((*obj)._pos, (*thrownObj)._pos)) / THROW_CNT_MAX;
						break;
					}
				}
			}
			else
			{
				(*thrownObj)._vel.x = player._throwRange / THROW_CNT_MAX;		// ����300��30�t���[����
			}
		}
		else
		{
			(*thrownObj)._vel.x = player._throwRange / THROW_CNT_MAX;		// ����300��30�t���[����
		}

		(*thrownObj).state(STATE::THROWN);

		// ������Ƃ��ɕ�����␳����
		//(*((Player&)actQue.second)._holdList.front())._rad = convToRad((*((Player&)actQue.second)._holdList.front())._dir);

		if ((*thrownObj)._vel.x == 0)
		{
			AST();
		}

		auto vel = (*thrownObj)._vel;
		vel.y = sqrt(THROW_INITIAL_SPEED*THROW_INITIAL_SPEED - vel.x * vel.x);
		(*thrownObj)._vel.y = vel.y;
		(*thrownObj)._gravity = 2 * ((*thrownObj)._height + vel.y * THROW_CNT_MAX) / (THROW_CNT_MAX * THROW_CNT_MAX);

		(*thrownObj)._holdPlayer.reset();
		(*thrownObj)._zOrder = 0;
		player._holdList.erase(player._holdList.begin());

		double height = 0.0;
		for (int i = 0; i < player._holdList.size();i++)
		{
			(*player._holdList[i])._height = static_cast<double>(player._size.y / 2) + height;
			height += (*player._holdList[i])._size.y;
		}
		player.state(STATE::THROW);
		return true;
	}
	else if (actQue.second.unitID() == UNIT_ID::POT)
	{
		// �₩��o��Ƃ�
		Pot& pot = ((Pot&)actQue.second);
		sharedObj thrownObj;

		double throwRad = 2.0 * PI / static_cast<double>(((Pot&)pot)._throwList.size());		// �f���o���Ƃ��̊p�x
		double cnt = 1.0;
		double radius = rand() % 100;

		for (auto selNum : pot._throwList)
		{
			for (auto obj : objList)
			{
				if ((*obj)._serialNum == selNum)
				{
					thrownObj = obj;

					(*thrownObj)._rad = throwRad * cnt;
					(*thrownObj).dir(convToDir(throwRad * cnt));
					(*thrownObj)._vel.x = (175.0 + radius) / THROW_CNT_MAX;
					(*thrownObj)._vel.y = sqrt(THROW_INITIAL_SPEED*THROW_INITIAL_SPEED - (*thrownObj)._vel.x * (*thrownObj)._vel.x);
					(*thrownObj)._gravity = 2 * ((*thrownObj)._height + (*thrownObj)._vel.y * THROW_CNT_MAX) / (THROW_CNT_MAX * THROW_CNT_MAX);
					(*thrownObj).state(STATE::THROWN);
					cnt += 1.0;
					break;
				}
			}
		}
		
		pot._throwList.clear();
		pot.state(STATE::THROW);

		return true;
	}
	else
	{
		AST();
		return false;
	}
}
