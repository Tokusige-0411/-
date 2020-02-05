#pragma once
#include "BaseScene.h"
#include "SceneMng.h"
#include <object/Obj.h>
#include <Input/InputState.h>
#include<object/PlNum.h>
#include<vector>

class EntrySceneLocal :
	public BaseScene
{
public:
	EntrySceneLocal();
	~EntrySceneLocal();

	UniqueBase Update(UniqueBase own)override;

private:
	void Init(void);
	void DrawInit(void);
	void EntryStart(void);
	void EntryNormal(void);

	void (EntrySceneLocal::*func)(void);

	int _padnum;								 // �ڑ�����pad��
	int _entryPadnum;							 // �G���g���[����pad��

	int _entrnum[2];							// �G���g���[�����ԍ���ۑ������`��

	int _entryBG;								// �G���g���[�w�i
	int _start;									// PUSHSTARTү����
	int _entry;
	int _entryImage[4][4];						// ����L������\���摜
	int _skyimage;								// �w�i�E��
	int _playerScreen[2];						// player�`��潸ذ�
	int _arrow[2];								// ���Image
	int _b;
	int _a;
	PlNum _plNum[2];							// �e��ڲ԰num�ۑ�
	bool _readyFlag[2];							// ���������׸�

	int _fadecolor;								// �t�F�[�h�̐��l
	int _fadespeed;								// �t�F�[�h���x
	int _cnt[2];								// �ڰѶ���


	std::shared_ptr<InputState> _input[4];		// ���͊Ǘ��N���X

	/*std::vector<std::shared_ptr<InputState>> _input;*/
};


