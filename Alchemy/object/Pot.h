#pragma once
#include <vector>
#include <tuple>
#include <functional>
#include <scene/SceneMng.h>
#include "Obj.h"
#include "FOLLOWER_TYPE.h"

#define COMBO_TIME_MAX 120					// ��ɓ���Ă���o�Ă���܂ł̎��� ��肠����1�b
#define COMBO_NUM_MAX 4						// �ő�R���{��

// �����X�^�[�̘B���Ɏg���f�[�^�Q
// UNIT_ID : Player�̎��ʂɎg��
// int     : �����X�^�[��TYPE��int�ɕϊ��������́APlayer�̏ꍇ��PLNUM�H
// ���ヂ���X�^�[�̃��A�x�Ȃǂ�ǉ��\��Ȃ̂�tuple
using MixData = std::tuple<UNIT_ID, int>;

using PotFunc = std::function<void(std::vector<MixData>&)>;

struct PotHold;
struct FuncThrow;
struct FuncIntoPot;

// �z���f�[�^�p�̍\����
struct MixCombination
{
	int confType;					// �g�ݍ��킹�ɂ���Ċm��ŏo�Ă���
	char  mixProb[10];				// �m���i�[�ꏊ
};

class Pot :
	public Obj
{
public:
	Pot();
	~Pot();
	void Update(std::vector<sharedObj>& objList)override;
	void AddMixList(MixData data);			// ��ɓ��������̂𔻕ʂ��Ĕr�o������̂�ς���
	std::vector<int>& followerCnt(void);

private:
	friend PotHold;
	friend FuncThrow;
	friend FuncIntoPot;
	void Init(void);
	void CombiListInit(void);

	int _comboCnt;							// ��ɕ��������Ă���̃J�E���g,��������x�Ƀ��Z�b�g
	int _comboNum;							// �R���{��
	int _gaugeID;							// �R���{�Q�[�W�̕`��X�N���[��
	int _gaugeImg[2];						// �R���{�Q�[�W�̉摜ID
	std::vector<int> _followerCnt;					// ��ɓ����Ă�t�H�����[�̐�
	std::vector<MixData> _mixList;			// ��ɓ����Ă����
	std::vector<MixCombination> _combiList;	// �z���̑g�ݍ��킹�̃��X�g
	std::vector<int> _throwList;			// �₩�炾�����̂�serialNum
	void InitFunc(void);					// AlchemyQue�̏�����
	std::map<UNIT_ID,PotFunc> AlchemyQue;	// ��B�����܂Ƃ߁H
	bool _inPotFlg;							// ��ɓ��������̪�ĊǗ��p
};

