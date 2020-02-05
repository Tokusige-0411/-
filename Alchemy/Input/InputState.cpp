#include <_DebugConOut.h>
#include "InputState.h"

InputState::InputState()
{
	// ���̓L�[�̐ݒ�
	KeyPair pair = { 0,1 };
	for (auto id : INPUT_ID())
	{
			_btnState.try_emplace(id, pair);
	}

	_LStickState.first = { false,90,DIR::DOWN };
	_LStickState.second = { true,90,DIR::DOWN };
}

InputState::~InputState()
{
}

// �L�[�}�b�v���Ǝ擾
const KeyMap & InputState::btnState(void) const
{
	return _btnState;
}

// �w�肵���L�[�̉�����Ԃ����擾
const KeyPair & InputState::btnState(INPUT_ID id) const
{
	static const KeyPair errorState = KeyPair(0, 0);		// �G���[���ɕԂ�
	try 
	{
		return _btnState.at(id);
	}
	catch (...)
	{
		AST();
		return errorState;
	}
}

// �L�[��Ԑݒ�
bool InputState::btnState(INPUT_ID id,char inputData)
{
	if (_btnState.find(id) != _btnState.end())
	{
		_btnState[id].first = inputData;
		return true;
	}
	
	return false;
}

const StickPair & InputState::LStickState(void)
{
	return _LStickState;
}

void InputState::LStickState(StickState& inputData)
{
	_LStickState.first = std::move(inputData);
}

void InputState::StateReset(void)
{
	for (auto id : INPUT_ID())
	{
		btnState(id, 0);
	}
	StickState state = { false,0,DIR::DOWN };
	LStickState(state);
}

// ���̓L�[��old��ԍX�V
void InputState::SetOld(void)
{
	for (auto id : INPUT_ID())
	{
		try
		{
			// first  : now
			// second : old
			_btnState[id].second = _btnState[id].first;
		}
		catch(...)
		{
			AST();
			_btnState.emplace(id, KeyPair{ 0,1 });
		}
	}

	_LStickState.second = _LStickState.first;
}

