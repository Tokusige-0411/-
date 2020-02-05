#pragma once
#include <memory>
#include "Obj.h"
#include "PlNum.h"
#include <Message.h>
#include <Input/INPUT_ID.h>

#define MENU_OFSET_X 504
#define MENU_OFSET_Y 310

class NetWork;
struct FuncThrow;
struct FuncHold;
struct FuncPut;
struct PlayerHold;

class Player :
	public Obj
{
public:
	Player();
	Player(PlNum plNum, Vector2Dbl pos, int padNum,sharedObj potObj);
	Player(PlNum plNum, Vector2Dbl pos, sharedObj potObj);
	void Update(std::vector<sharedObj>& objList) override;
	PlNum plNum(void);
	static int GetpCnt(void);
	static void ReSetpCnt(void);
	int holdWeightMax(void);
	int GetHoldListSize(void);
	void MakeBtnGuide(INPUT_ID btn);						// �{�^���K�C�h�̉摜�����DrawQue�𓊂���
	void MakeBtnGuide(INPUT_ID btn, Vector2Dbl drawPos);	// ���W�w���
	bool throwPot(void);
	const int checkNum(void)const;
	void throwPot(bool throwMode);
	void DoDamage(int power) override;
	const Vector2Dbl& ShakeOffset(void);

	~Player();
private:
	friend NetWork;
	friend FuncHold;
	friend FuncThrow;
	friend FuncPut;
	friend PlayerHold;
	void Init(void);
	void DrawHP(void)override;
	void Menu(void);

	static bool _menuFlag[2];					// �ƭ���\�����邩�ǂ���
	bool _throwPot;					// �����Ă�Ƃ��ɚ���^�[�Q�b�g�ɂ��邩�ǂ���
	int _holdWeightMax;				// ���Ă�d�����
	double _throwRange;				// �����̋���
	int _playerHPImg[3];		// �v���C���[�p��HP�摜
	int _buttonID;
	int _cursorNum;					// ���j���[�J���Ă鎞�̃J�[�\���̈ʒu
	static int _playerIcon[4][2];	//
	static int pCnt;
	int _checkNum;						// ������ڲ԰��playre����
	PlNum _plNum;
	std::vector<sharedObj> _holdList;			// ���ݎ����Ă���I�u�W�F�N�g
	int _shakeCount;					// �������鎞��
	Vector2Dbl _shakeOffset;			// �����������W�̵̾��
};

