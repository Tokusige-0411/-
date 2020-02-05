#pragma once
#include "object/Obj.h"

#define HEIGHT_CNT_MAX 120.0
#define ADD_CNT_MAX	60;

class Boss :
	public Obj
{
public:
	Boss();
	Boss(sharedObj potObj);
	~Boss();
	void Update(std::vector<sharedObj>& objList) override;

private:
	void Init();
	bool AddBossMove(void);				// �ݽ�ݽ����ďo������Ƃ��̓���
	bool AddBossHP(void);				// Hp
	void DrawHP(void)override;			// Hp�`��p
	void BossDeath(void);				// �޽���S�̪��

	void AddBoss(void);					// �޽�o��

	int _bossHPImg[3];					// �޽��HP�摜�p
	int _heightCnt;						// �o�����̏ォ�痎���Ă���Ƃ��Ɏg��
	int _bossHPCount;					// HP��������������
	int _addCount;						// �o�����p�̶���
	double _addVec;						// �o�����̽�߰��
	bool _addFlag;						// �o�����p�׸�
	STATE oldState;
	int deathScreen;					// �������ł�������
	int _deathCnt;
};

