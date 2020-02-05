#pragma once
#include "BaseScene.h"
#include "SceneMng.h"
#include <object/Obj.h>
#include <Input/InputState.h>
#include<object/PlNum.h>
#include<vector>

class EntryScene :
	public BaseScene
{
public:
	EntryScene();
	~EntryScene();

	UniqueBase Update(UniqueBase own)override;

private:
	void Init(void);
	void DrawInit(void);
	void EntryStart(void);
	void EntryNormal(void);

	void (EntryScene::*func)(void);

	int _padnum;								 // �ڑ�����pad��
	int _entryPadnum;							 // �G���g���[����pad��

	bool _entrnum;							// �G���g���[�����ԍ���ۑ������`��

	int _entryBG;								// �G���g���[�w�i
	int _backImage[4];							// �ڑ��ςݖ��o�^�̔w�i
	int _entryImage[4][4];						// ����L������\���摜
	int _skyimage;								// �w�i�E��

	int _fadecolor;								// �t�F�[�h�̐��l
	int _fadespeed;								// �t�F�[�h���x
	int _cnt;								// �ڰѶ���


	std::shared_ptr<InputState> _input;		// ���͊Ǘ��N���X

	/*std::vector<std::shared_ptr<InputState>> _input;*/
};

