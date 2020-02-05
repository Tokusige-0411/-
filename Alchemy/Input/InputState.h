#pragma once
#include <map>
#include "INPUT_ID.h"
#include <Vector2.h>
#include <object\Obj.h>
#include <DIR.h>

using KeyPair = std::pair<char, char>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

struct StickState
{
	bool isInput;		// ���͂����邩
	short angle;		// �X�e�B�b�N�̊p�x
	DIR dir;			// �������ɂ����Ƃ��̌���
};

using StickPair = std::pair<StickState, StickState>;

class InputState
{
public:
	InputState();

	virtual ~InputState();

	const KeyMap& btnState(void)const;					// �L�[�}�b�v���Ǝ擾
	const KeyPair & btnState(INPUT_ID id)const;			// �w�肵���L�[�̉�����Ԃ����擾
	bool btnState(INPUT_ID id, char inputData);			// �L�[��Ԑݒ�

	const StickPair& LStickState(void);
	void LStickState(StickState& inputData);
	void StateReset(void);

	void SetOld(void);									// �L�[��old��ԍX�V
	virtual void Update(std::vector<sharedObj>& objList) = 0;

private:
	KeyMap _btnState;									// ���͉\�L�[�̉������,first:new,second:old
	StickPair _LStickState;								// ���X�e�B�b�N�̓��͏��,first:new,second:old
};