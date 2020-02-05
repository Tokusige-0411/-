#pragma once
#include <vector>
#include <object\Obj.h>
#include <Vector2.h>

enum class MOVE_TYPE
{
	CHARGE,		// �����
	SHOT,		// ����
	BLAST		// ����
};

#define MAX_CNT 60.0		// �ő嶳�Ăƒe�̽�߰��

struct GhostAttack
{
public:
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);

private:
	// �e�̪�Ă̓���
	void Charge(Obj& obj);								// �����
	void Shot(Obj& obj);								// ����
	void Blast(Obj& obj);								// ����

	bool IsHitObj(Vector2Dbl shot, Vector2Dbl targetPos,Vector2Int targetSize);	// �����ő���

	MOVE_TYPE _move = MOVE_TYPE::CHARGE;				// �ꏊ���Ȃ����珉��������
	double _endCnt;										// �e����足�ē�����Ȃ�����������Đ؂�ւ���
	Vector2Dbl _shotPos;								// �e�̍��W
	Vector2Dbl _shotSpeed;								// �e�̽�߰�� 
};

