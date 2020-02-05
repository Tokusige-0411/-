#pragma once
#include "BaseScene.h"
#include <object/Obj.h>
#include <Input/InputState.h>
#include<common/DIR.h>

enum class SELECTMENU
{
	START,
	END,
	MAX
};

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	UniqueBase Update(UniqueBase own)override;

private:
	void Init(void);
	void TitleSelect(void);
	void DrawInit(void);
	void SoundInit(void);
	void TitleEnd(void);
	void TitleNormal(void);
	void LAST(void);

	void (TitleScene::*func)(void);

	int _titleLogo;								// �^�C�g�����S
	int _titleBG;								// �ʏ�^�C�g���w�i
	int _cursor;								// �J�[�\���摜
	int _selectTex_1;							// GAME START,GAME END
	int _titleTex;								// PLEES START
	int _skyimage[2];							// �w�i�E��

	Vector2Dbl T_logoPos;						// �^�C�g�����S�̍��W
	Vector2Dbl _skyPos[2];						// ��̍��W
	Vector2Dbl _cursorPos;						// �J�[�\���ʒu���W

	int _padnum;								// �ڑ�����pad��

	DIR _oldDir;
	DIR _newDir;


	double _fallSpeed;							// �Z���N�g���j���[�̈ړ����x
	int _cnt;									// �ڰѶ���
	int _fadecolor;								// �t�F�[�h�̐��l
	int _fadespeed;								// �t�F�[�h���x

	SELECTMENU _selectnum;						// �Z���N�g���e

	std::shared_ptr<InputState> _input[4];		// ���͊Ǘ��N���X

};

