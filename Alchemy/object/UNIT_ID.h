#pragma once

// �I�u�W�F�N�g�̎��
enum class UNIT_ID
{
	NON,
	PLAYER,
	ENEMY,
	BOSS,
	ITEM,
	POT,
	FOLLOWER
};

UNIT_ID bigin(UNIT_ID());
UNIT_ID end(UNIT_ID());
UNIT_ID operator*(UNIT_ID key);
UNIT_ID operator++(UNIT_ID& key);

