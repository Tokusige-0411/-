#pragma once
#include "../GameScene.h"

#define THROW_CNT_MAX 30.0			// �����Ă��璅�n�܂ł̎���
#define THROW_INITIAL_SPEED 15.0	// �����̏����A_vel��ݒ肷�鎞�̑傫��

// Player�����𓊂���Ƃ��̏���
struct FuncThrow
{
	bool operator()(ActQueT&, std::vector<sharedObj>&);
};

