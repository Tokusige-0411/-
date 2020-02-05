#pragma once
#include <vector>
#include "Text.h"

class Message
{
public:
	Message();
	~Message();
	void AddMesList(Text& mes);			// ���X�g�Ƀe�L�X�g��ǉ�����
	int mesScreen(void);
	void DrawUpdate(void);

private:
	int _mesScreen;					// ���b�Z�[�W��`�悷��X�N���[��
	int _boardImg;
	int _mesMoveCnt;
	std::vector<Text> _mesList;		// �����Ă������b�Z�[�W���i�[����
};

