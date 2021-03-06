#pragma once
#include "NetWork.h"
class Host :
	public NetWorkUnit
{
public:
	Host(int port);
	~Host();
	bool GetData(void)override;
	int netWorkHandle(PlNum num);
	void Connect(void);
	IPDATA ip(PlNum num);
	PlNum GetPlNum(void);						// 現在通信しているplayerのnum
	void RunMesList(void)override;				// ﾒｯｾｰｼﾞ送信
	void StartGame(void);						// ｹﾞｰﾑｽﾀｰﾄMES作成
private:
	void TransferMes(void);					// ﾒｯｾｰｼﾞの転送
	bool CheckDisConnect(void);					// 切断したやつがいるか
	void ConnectCheck(void);					// IPｱﾄﾞﾚｽ取得及び接続完了ﾒｯｾ送信
	void Update(void);
};

