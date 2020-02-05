#pragma once

enum class FOLLOWER_TYPE
{
	OCTOPUS = 1,		// ����
	BEE = 2,		// �͂�
	GHOST = 4,		// ���΂�
	MUSH = 8,		// ���̂�
	DAEMON = 16,		// �ް��
	POT = 32,			// �ذ
	MAX
};


FOLLOWER_TYPE begin(FOLLOWER_TYPE);
FOLLOWER_TYPE end(FOLLOWER_TYPE);

FOLLOWER_TYPE operator++(FOLLOWER_TYPE type);
FOLLOWER_TYPE operator*(FOLLOWER_TYPE type);
