#include "PotAttack.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>

void PotAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
{
	// �����N�؂�
	if (obj.tageObj().expired())
	{
		StopEffekseer2DEffect(obj._effectID);
		obj._effectFlg = false;
		obj._coolCnt = obj._coolCntMax;
		obj.state(STATE::NORMAL);
		return;
	}

	auto target = obj.tageObj().lock();					// �^�[�Q�b�g�̏��i�[

	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			(*target).DoDamage(obj._power);				// ��Ұ�ނ���������
		}
		else
		{
			obj._coolCnt--;								// ������

			// �X�e�[�^�X��߂�
			if (obj._coolCnt <= 0)
			{
				StopEffekseer2DEffect(obj._effectID);
				obj._coolCnt = obj._coolCntMax;
				obj._effectFlg = false;
				obj.state(STATE::NORMAL);
				return;
			}
			return;										// �U�����I����Ă邩�珈���͂����܂�
		}
	}
	else
	{
		obj._effectFlg = true;
	}

	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj,(*target)._pos,0,EFFECT::POT,(*target)._zOrder + 1 });
}
