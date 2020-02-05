#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <memory>
#include <_debug/_DebugConOut.h>
#include <scene/SceneMng.h>
#include <object/Player.h>
#include <object/PlNum.h>
#include "../Input/InputState.h"
#include "NetWorkUnit.h"

enum class NETMODE
{
	OFFLINE,			// ��ײ�
	HOST,				// νď��
	GUEST,				// �ײ��ď��
	MAX
};


#define lpNetWork NetWork::GetInstans()
#define SYNC_TIME 30


class NetWork
{
public:
	static NetWork &GetInstans(void)
	{
		return *sInstans;
	}

	bool NetMode(NETMODE mode);
	const NETMODE NetMode(void);
	bool Start(void);											// �J�n���Ă�����
	bool Active(void);											// �ڑ����Ă��邩
	void Connect(void);											// �ڑ������ɍs��
	void UpDate(void);											// �ڑ�������͂�����񂵂Ă�΂����͂�
	void ReSetRecMes(void);										// ��Mү���ލ폜
	void SyncObj(Player& obj);									// ����ү���ފl�����ʒu����
	MES GetMes(MES_TYPE type);									// �w�肳�ꂽtype��ү���ގ擾
	MES GetMes(PlNum num,MES_TYPE type);						// �w�肳�ꂽPlNum,type��ү���ގ擾
	void GetKey(std::vector<MES>& buf,PlNum num );				// _recMesList����̷����̎��o��
	PlNum GetPlNum(void);										// ���݂̎�����PlNum�擾
	void MakeKeyMes(KeyMap butan, StickState& stick);			// Key�������ү���މ�
	void MakeAgainMes(PlNum plNum ,unsigned int num);			// �đ��˗�ү���ލ쐬
	void MakeSyncMes(Vector2Dbl pos);							// �����pү���ލ쐬
private:
	struct NetWorkDeleter
	{
		void operator()(NetWork* netWork)const
		{
			delete netWork;
		}
	};
	static std::unique_ptr<NetWork, NetWorkDeleter> sInstans;
	NetWork();
	virtual ~NetWork();
	unsigned char _keyNum;										// �����m�F�p�ʂ��ԍ�
	int _syncTime;										// �����̂��߂̶���
	MES _tmpMes;										// �󂯎��MES�ꎞ�ۑ��p
	NETMODE _netMode;									// ȯ�ܰ���Ӱ�� host��Guest��
	std::unique_ptr<NetWorkUnit> _netWorkUnit;			// �ư��߲��
};

