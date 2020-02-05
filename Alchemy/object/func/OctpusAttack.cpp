#include "OctpusAttack.h"
#include "BatAttack.h"
#include <DxLib.h>
#include <math.h>
#include <EffekseerForDXLib.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>

void OctpusAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
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
	int _effectzOrder;									// �ޯĵ��ް
	Vector2Dbl _ofSet = { 0,0 };						// ���W�̵̾��

	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			(*target).DoDamage(obj._power);				// ��Ұ�ނ���������
			_effectRad = atan2((*target).pos().y - obj._pos.y, (*target).pos().x - obj._pos.x);
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


	// �̾�ĂƵ��ް�̒���
	switch (obj._dir)
	{
	case DIR::UP:
		_effectzOrder = obj._zOrder - 1;
		_ofSet.y -= obj._size.y / 2.0;
		break;
	case DIR::LEFT:
		_effectzOrder = obj._zOrder;
		_ofSet.x -= obj._size.x / 2.0;
		break;
	case DIR::RIGHT:
		_effectzOrder = obj._zOrder;
		_ofSet.x = obj._size.x / 2.0;
		break;
	case DIR::DOWN:
		_effectzOrder = obj._zOrder;
		break;
	default:
		break;
	}

	// �X�e�[�^�X��߂�
	if (obj._coolCnt <= 0)
	{
		StopEffekseer2DEffect(obj._effectID);
		obj._coolCnt = obj._coolCntMax;
		obj.state(STATE::NORMAL);
		return;
	}

	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj,obj._pos + _ofSet,_effectRad,EFFECT::INK,_effectzOrder });
}
