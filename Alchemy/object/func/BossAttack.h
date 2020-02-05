#pragma once
#include <vector>
#include <object\Obj.h>
#include <EFFECT.h>

enum class MOVE_TYPE
{
	CHARGE,
	BLAST
};

struct BossAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
private:
	// �e�̪�Ă̓���
	void Charge(Obj& obj);									// �����
	void Blast(Obj& obj, std::vector<sharedObj>& objList);	// ����

	MOVE_TYPE _move = MOVE_TYPE::CHARGE;					// �ꏊ���Ȃ����炱���ŏ���������
	int _chargeCnt = 0;										// ����޶���
	Vector2Dbl _effectPos;									// �õ�̍��W
	EFFECT _type;						
};