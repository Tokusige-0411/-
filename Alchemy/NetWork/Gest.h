#pragma once
#include <DxLib.h>
#include "NetWork.h"

#define WAIT_TIME 6			// 通信待機時間

class Guest :
	public NetWorkUnit
{
public:
	Guest(IPDATA IP,int port);
	~Guest();
	IPDATA ip(PlNum num);
	int netWorkHandle(PlNum num);
	void Connect(void);
	bool GetData(void)override;
	PlNum GetPlNum(void);
private:
	void Update(void);
};

