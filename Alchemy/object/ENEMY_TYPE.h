#pragma once

enum class ENEMY_TYPE
{
	SLIME = 1,		// �ײ�
	BAD = 2,		// ����
	GHOST = 4,		// ���΂�
	MUSH = 8,		// ���̂�
	DAEMON = 16,	// �ް��
	BOSS = 32,		// �޽
	MAX
};


ENEMY_TYPE begin(ENEMY_TYPE);
ENEMY_TYPE end(ENEMY_TYPE);

ENEMY_TYPE operator++(ENEMY_TYPE type);
ENEMY_TYPE operator*(ENEMY_TYPE type);