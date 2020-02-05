#pragma once
#include <memory>
#include<vector>
#include <object\Obj.h>
//#include 

class BaseScene;

enum class ACT_QUE
{
	NON,
	ATTACK,			// �U��
	HOLD,			// ����
	THOROW,			// ������
	INTO_POT,		// �����蔻��
	PUT,			// ���̏�ɒu��
};

enum class INSTANCE_QUE
{
	ID,
	TYPE,
	POS,
	NUM
};

using UniqueBase = std::unique_ptr<BaseScene>;
using ActQueT = std::pair<ACT_QUE, Obj&>;
using InstanceQueT = std::tuple<UNIT_ID, int, Vector2Dbl,int>;		// id�A���߁A���W�A�ʂ��ԍ�

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual UniqueBase Update(UniqueBase own) = 0;
	virtual void RunActQue(std::vector<ActQueT> actList);
	virtual void RunInstanceQue(std::vector<InstanceQueT> instanceList);
	Vector2Dbl _screenPos;
protected:
	std::vector<sharedObj> _objList;
};

