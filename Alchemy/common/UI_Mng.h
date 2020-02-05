#pragma once
#include <tuple>
#include <vector>
#include <memory>
#include <Message.h>

#define lpUI_Mng UI_Mng::GetInstance()

// UI�ɕ`�悵��������
// x
// y
// image
using UIQue = std::tuple<double, double, int>;

class UI_Mng
{
public:
	// �ݽ�׸�
	static UI_Mng & GetInstance(void)
	{
		return *_sInstance;
	}

	void AddMessage(Text mes);
	void AddUIQue(UIQue que);
	void Draw(void);
	void UI_Init(void);
	void UI_ReSet(void);
	int FadeCnt(void);
	void FadeCnt(int cnt);
	bool FadeFlag(void);
	int screen(void);

private:
	struct UI_MngDeleter
	{
		void operator()(UI_Mng* uiMng) const
		{
			delete uiMng;
		}
	};
	UI_Mng();
	~UI_Mng();

	int _screen;
	int _btnGuideImg;
	int _barImg;
	int _blackImg;

	int _fadeCnt;
	bool _fadeFlag;		// true�̎������J�����������Ăق����Ȃ�

	std::unique_ptr<Message> _mesBoard;
	std::vector<UIQue> _uiList;

	static std::unique_ptr<UI_Mng, UI_MngDeleter> _sInstance;

};
