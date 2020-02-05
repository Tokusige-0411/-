#pragma once
#include <vector>
#include "../Obj.h"

class Player;

struct PlayerHold
{
	void operator()(Obj&, std::vector<sharedObj>& objList);
private:
	// ����������ւ��ړ�
	void ExchangeMove(Player&);

	bool _exChange;		// ����������ւ����̓���ړ��׸�
	int _changeNum;		// ����ւ��Ă�I�u�W�F�N�g�̊i�[�ꏊ
	int _moveCnt;		// ����ւ��ړ��̃J�E���g
	std::vector<std::pair<double,double>> _heightList;	// ����ւ��O��̍��������A�i�[�ꏊ�͓���ւ����holdList�ƑΉ����Ă���
};
