#pragma once

enum class EFFECT
{
	AT_BITE,			// ���݂�
	HEAL,				// ��
	AT_SLIME,			// �ײ�
	GHOST_CHARGE,		// �����
	GHOST_SHOT,			// �e�\���p
	GHOST_BLAST,		// ����
	DEMON_CHARGE,		// �����
	DEMON_WAR,			// �x��
	INK,				// �n�U��
	FIRE,				// ���U��
	AT_BEE,				// �I�̂�������
	SMOKE,				// �ɺ�̉��U��
	LANDING,			// ���n
	DARK,				// �޽�U���P
	BOSS_WAR,			// �޽�����
	METEO,				// �߂Ă�
	FIRE_TWO,			
	DEATH,
	POT,				// �~�j�c�{�̂�������
	BOHU,				// �ڂӂ�
	MAX
};

EFFECT begin(EFFECT);
EFFECT end(EFFECT);

EFFECT operator ++(EFFECT& effect);
EFFECT operator *(EFFECT effect);