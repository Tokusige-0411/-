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
	PlNum GetPlNum(void);						// ���ݒʐM���Ă���player��num
	void RunMesList(void)override;				// ү���ޑ��M
	void StartGame(void);						// �ްѽ���MES�쐬
private:
	void TransferMes(void);					// ү���ނ̓]��
	bool CheckDisConnect(void);					// �ؒf����������邩
	void ConnectCheck(void);					// IP���ڽ�擾�y�ѐڑ�����ү����M
	void Update(void);
};

