#include "MushroomAttack.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>

void MushroomAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
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

	auto target = obj.tageObj().lock();							// �^�[�Q�b�g�̏��i�[
	Vector2Dbl _ofSet;											// �̾��
	Vector2Dbl _effectPos;										// �U�����񕜂œn�����W���Ⴄ


	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			// ���ޯĂ��G���������ŕς��
			if ((*target).teamTag() == obj.teamTag())
			{
				(*target).DoDamage(-obj._power);				// ��
				_id = EFFECT::HEAL;
				_effectRad = 0;
			}
			else 
			{
				(*target).DoDamage(obj._power);					// ��Ұ�ނ���������
				_id = EFFECT::SMOKE;
				_effectRad = atan2((*target).pos().y - obj._pos.y, (*target).pos().x - obj._pos.x);
			}
		}
		else
		{
			obj._coolCnt--;										// ������

			// �X�e�[�^�X��߂�
			if (obj._coolCnt <= 0)
			{
				StopEffekseer2DEffect(obj._effectID);
				obj._coolCnt = obj._coolCntMax;
				obj._effectFlg = false;
				obj.state(STATE::NORMAL);
				return;
			}
			return;												// �U�����I����Ă邩�珈���͂����܂�
		}
	}
	else
	{
		obj._effectFlg = true;
	}

	// ����ɕK�v�ȕϐ��̐ݒ�
	if (_id == EFFECT::HEAL)
	{
		_effectPos = (*target)._pos;
		_ofSet = { 0,(*target)._size.y / 2.0 };						// ��������̪�ďo�Ăق���

	}
	else if (_id == EFFECT::SMOKE)
	{
		_effectPos = obj._pos;
		_ofSet = { 0,0 };
	}


	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj,_effectPos + _ofSet,_effectRad,_id,(*target)._zOrder + 1});
}
