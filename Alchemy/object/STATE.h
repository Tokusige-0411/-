#pragma once
// �I�u�W�F�N�g�̏��
enum class STATE
{
	NORMAL,		// �ʏ�
	HOLD,		// �����Ă���
	THROW,		// ����
	ATTACK,		// �U��
	THROWN,		// �������Ă���
	HOLDEN,		// ������Ă���
	DEATH,		// ��
	MAX,
};


STATE begin(STATE);
STATE end(STATE);

STATE operator++(STATE type);
STATE operator*(STATE type);
