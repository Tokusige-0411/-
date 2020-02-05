#include "NetState.h"

NetState::NetState(PlNum plNum/*, Obj& obj*/)
{
	_missingCnt = 0;
	_searchFlag = false;
	_plNum = plNum;
	_num = 0;
}

NetState::~NetState()
{
}

void NetState::Update(std::vector<sharedObj>& objList)
{
	if (!lpNetWork.Start())
	{
		return;
	}
	if (!_searchFlag)
	{
		
	}
	SetOld();
	// ү���ގ��o���p�ϐ��쐬 ү���ނȂ������ꍇ�̂��߂�Old���
	StickState state;
	// ү���ގ擾
	lpNetWork.GetKey(_keyBuf, _plNum);
	if (_keyBuf.size() == 0)
	{
		return;
	}
	if (_keyBuf.size() <= _num)
	{
		TRACE("-------------------------------------------------------------------------------\n");
		_missingCnt++;
		TRACE("%d\n", _missingCnt);
		return;
	}
	btnState(INPUT_ID::BTN_A, _keyBuf[_num].key.a);
	btnState(INPUT_ID::BTN_B, _keyBuf[_num].key.b);
	btnState(INPUT_ID::BTN_Y, _keyBuf[_num].key.y);
	btnState(INPUT_ID::BTN_LB, _keyBuf[_num].key.lb);
	btnState(INPUT_ID::BTN_RB, _keyBuf[_num].key.rb);
	btnState(INPUT_ID::LEFT_TRIGGER, _keyBuf[_num].key.lt);
	btnState(INPUT_ID::RIGHT_TRIGGER, _keyBuf[_num].key.rt);
	btnState(INPUT_ID::UP, _keyBuf[_num].key.up);
	btnState(INPUT_ID::DOWN, _keyBuf[_num].key.down);
	if (_keyBuf[_num].key.lf)
	{
		state.isInput = true;
		state.angle = _keyBuf[_num].key.ls;
		state.dir = convToDir(_keyBuf[_num].key.ls);
	}
	else
	{
		state.isInput = false;
		state.isInput = 0;
		state.dir = DIR::DOWN;
	}
	LStickState(state);
	_num++;
	if (_keyBuf.size() > 256 && _num >= 256)
	{
		for (auto i = 0; i < 256; i++)
		{
			if (_keyBuf[i].key.num != i)
			{
				AST();
			}
		}
		_keyBuf.erase(_keyBuf.begin(), _keyBuf.begin() + 256);
		_num = 0;
	}
}

void NetState::SearchObj(std::vector<sharedObj>& List)
{
	for (auto obj : List)
	{
		if ((*obj).unitID() == UNIT_ID::PLAYER)
		{
			if (((Player&)(*obj)).plNum() == _plNum)
			{
				_plObj = obj;
				_searchFlag = true;
				return;
			}
		}
	}
}
