#include "Host.h"

static PlNum pCnt = PlNum::PL_NON;

Host::Host(int port)
{
	_port = port;
	_plNum = PlNum::PL_01;
}


Host::~Host()
{
	if (_netWorkHandle.size())
	{
		for (PlNum num = begin(PlNum()); num < _plNum; ++num)
		{
			CloseNetWork(netWorkHandle(num));
		}
	}
}

bool Host::GetData(void)
{
	for (PlNum num = begin(PlNum()); num < _plNum; ++num)
	{
		while (CheckData(num))
		{
			AddRecMesList(_mes);
		}
	}
	return true;
}

int Host::netWorkHandle(PlNum num)
{
	try
	{
		return _netWorkHandle.at(num);
	}
	catch (...)
	{
		AST();
		return 0;
	}
}

IPDATA Host::ip(PlNum num)
{
	try
	{
		return _ip.at(num);
	}
	catch (...)
	{
		AST();
		return IPDATA{ 0,0,0,0 };
	}
}

PlNum Host::GetPlNum(void)
{
	return _plNum;
}

void Host::Connect(void)
{
	// �ڑ��҂���Ԃֈڍs
	PreparationListenNetWork(_port);
	// �A���Ă���NetWorkHandle��-1����Ȃ��Ȃ�true
	if (SetNetWorkHandle(GetNewAcceptNetWork()))
	{
		// ip���ڽ�l��&�ڑ��������ް����M
		ConnectCheck();
		GetData();
		// 4�l����H
		if (_plNum == PlNum::PL_MAX)
		{
			// 4�l�ڑ������Őڑ��I��
			StartGame();
		}
	}
}


void Host::TransferMes(void)
{
	auto check = [](MES mes) {
		if (static_cast<MES_TYPE>(mes.check.type) == MES_TYPE::KEY)
		{
			return true;
		}
		//else if (static_cast<MES_TYPE>(mes.check.type) == MES_TYPE::SYNC)
		//{
		//	return true;
		//}
		else
		{
			return false;
		}
	};
	if (_recMesList.size() == 0)
	{
		return;
	}
	for (auto data:_recMesList)
	{
		if (!check(data))
		{
			continue;
		}
		AddSendMesList(data);
	}
}

bool Host::CheckDisConnect(void)
{
	// �ؒf����������邩
	int DisCnHandle = GetLostNetWork();
	auto specificPlayer = [&]() {
		for (PlNum num = begin(PlNum()); num < _plNum; ++num)
		{
			if (netWorkHandle(num) == DisCnHandle)
			{
				return num;
			}
		}
		return PlNum::PL_NON;
	};
	if (DisCnHandle == -1)
	{
		return false;
	}
	TRACE("player[%d]���񂪃��O�A�E�g���܂���\n",(static_cast<int>(specificPlayer())+1));
	return true;
}

void Host::ConnectCheck(void)
{
	CONNECT_INF tmpInf = {
		static_cast<unsigned char>(MES_TYPE::CONNECT),
		static_cast<unsigned char>(_plNum)
	};
	// ���Ԗڂ�Pl��ү���ލ쐬
	MES tmpMes;
	tmpMes.connect = tmpInf;
	// �ؒf���Ă��܂������p��IP���ڽ�m��
	GetNetWorkIP(netWorkHandle(_plNum), &_ip[_plNum]);
	// �ڑ��m�F������ү����M
	DataSend(netWorkHandle(_plNum), tmpMes);
	TRACE("player[%d]���񂪐ڑ����܂���\n", (static_cast<int>(_plNum) + 1));
	_linkFlag = true;
	++_plNum;
	TRACE("%d\n", static_cast<int>(_plNum));
}

void Host::RunMesList(void)
{
	if (_sendMesList.size() == 0)
	{
		return;
	}
	// �eHandle�ɑ��M
	for (PlNum num = begin(PlNum()); num < _plNum; ++num)
	{
		for (unsigned int i = 0; i < _sendMesList.size(); i++)
		{
			DataSend(netWorkHandle(num), _sendMesList[i]);
		}
	}
	_sendMesList.clear();
}

void Host::StartGame(void)
{
	// �V�����ڑ����I��
	StopListenNetWork();
	for (PlNum num = PlNum::PL_01; num < _plNum; ++num)
	{
		TYPE_INF tmpInf = {
			static_cast<unsigned char>(MES_TYPE::GAMEMODE),
			static_cast<unsigned char>(num)
		};
		// �ްѽ���ү���ލ쐬
		MES tmpMes;
		tmpMes.check = tmpInf;
		AddSendMesList(tmpMes);
	}
	_GameFlag = true;
}

void Host::Update(void)
{
	// �ؒf����
	CheckDisConnect();
	RunMesList();
	GetData();
	TransferMes();
}
