#pragma once
#include <map>
#include <memory>
#include <tuple>
#include <vector>
#include <Vector2.h>
#include "BaseScene.h"
#include "LAYER.h"
#include<object/PlNum.h>

class SceneMng;

#define lpSceneMng SceneMng::GetInstance()

enum class DRAW_QUE
{
	IMAGE,
	X,
	Y,
	RAD,
	RATE,
	HEIGHT,
	ZORDER,
	LAYER,
	MODE,
	PARAMETER
};

enum class SOUND_QUE
{
	SOUND,
	VOL,
	X,
	Y
};

enum class PLAYER_QUE
{
	PLNUM,
	POS,
	PADNUM
};

using DrawQueT = std::tuple<int, double, double, double, double,double, int, LAYER, int, int>;
using SoundQueT = std::tuple<int, int, double, double>;							// <�����, ����, �������WX, �������WY>
using PlayerQueT = std::tuple <PlNum, Vector2Dbl, int >;						// <��ڲ԰�ԍ��A���W�APAD�ԍ�>

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		return *sInstance;
	}

	void Run(void);										// �ް�ٰ��

	bool AddDrawQue(DrawQueT dQue);						// �`�淭��̒ǉ�
	bool AddActQue(ActQueT aQue);						// ����ݷ���̒ǉ�
	bool AddSoundQue(SoundQueT sQue);					// ����޷���̒ǉ�
	bool AddPlayerQue(PlayerQueT pQue);					// ��ڲ԰����̒ǉ�
	bool AddInstanceQue(InstanceQueT iQue);				// �ݽ�ݽ����̒ǉ�
	bool SetInstanceQue(InstanceQueT iQue);				// �ݽ�ݽ����̒ǉ�

	const size_t GetPlayerListSize(void)const;			// ��ڲ԰����̻��ގ擾
	std::vector<PlayerQueT>& playerList(void);			// ��ڲ԰����擾

	void FrameCntReset(void);
	const int frameCnt(void)const;
	int serialNumCnt(void);
	void AddSerialNum(void);							// �ʂ��ԍ����C���N�������g
	void SetShakeCount(int cnt);						// �������Ă�Ă���
	void SetGameClearFlag(bool flag);
	bool& GameClearFlag(void);
	void SetGameEndFlag(bool flag);
	bool& GameEndFlag(void);
	
	const Vector2Dbl ScreenSize;
	const Vector2Dbl WorldSize;							// ���E����
	const Vector2Dbl ScreenCenter;						// ��ʒ���
	const Vector2Dbl WorldCenter;
	

private:
	struct SceneMngDeleter
	{
		void operator()(SceneMng* sceneMng)const
		{
			delete sceneMng;
		}
	};

	SceneMng();
	~SceneMng();


	static std::unique_ptr<SceneMng, SceneMngDeleter> sInstance;

	void Draw(void);
	void SoundPlay(void);
	bool SysInit(void);

	int _layerGID;							// �`��p��ذ�
	int _frameCnt;							// �ڰѶ���
	int _serialNumCnt;						// �ʂ��ԍ��Ǘ��ϐ�

	std::map<LAYER, int> _screenID;

	std::vector<DrawQueT> _drawList;		// �`�淭��̊Ǘ��z��
	std::vector<ActQueT> _actList;			// ����ݷ���̊Ǘ��z��
	std::vector<SoundQueT> _soundList;		// ����޷���̊Ǘ��z��
	std::vector<PlayerQueT> _playerList;	// ��ڲ԰����̊Ǘ��z��
	std::vector<InstanceQueT> _instanceList;// �ݽ�ݽ����̊Ǘ��z��

	Vector2Dbl _screenPos;					// �����Ƃ��Ŏg����
	int _shakeCount;						// ��������

	bool _gameEndFlag;
	bool _gameClearFlag;					// �ر���ްѵ��ް��

	UniqueBase _activeScene;
};

