#include "DemonAttack.h"
#include <DxLib.h>
#include <math.h>
#include <algorithm>
#include <EffekseerForDXLib.h>
#include <Vector2.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>
#include <_DebugConOut.h>

void DemonAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
{
	// �����N�؂�
	if (obj.tageObj().expired())
	{
		TRACE("attack�̑Ώۂ������܂���")
		StopEffekseer2DEffect(obj._effectID);
		obj._effectFlg = false;
		obj._coolCnt = obj._coolCntMax;
		_move = MOVE_TYPE::CHARGE;
		obj.state(STATE::NORMAL);
		return;
	}

	switch (_move)
	{
	case MOVE_TYPE::CHARGE:
		Charge(obj);
		break;
	case MOVE_TYPE::BLAST:
		Blast(obj, objList);
		break;
	default:
		AST();
		break;
	}

}

void DemonAttack::Charge(Obj & obj)
{
	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			_chargeCnt++;
		}
		else
		{
			// 2��x���o�����玟�̍U���Ɉړ�
			if (_chargeCnt >= 2)
			{
				StopEffekseer2DEffect(obj._effectID);
				obj._effectFlg = false;
				obj._coolCnt = obj._coolCntMax;
				_chargeCnt = 0;
				_move = MOVE_TYPE::BLAST;
				return;
			}
			obj._effectFlg = false;
			return;
		}
	}
	else
	{
		obj._effectFlg = true;
	}

	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj,obj._pos,0,EFFECT::DEMON_WAR,obj._zOrder + 1 });
}

void DemonAttack::Blast(Obj & obj, std::vector<sharedObj>& objList)
{
	auto target = obj.tageObj().lock();					// �^�[�Q�b�g�̏��i�[

	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			// ��������̋������߂�����objList���\�[�g
			std::sort(objList.begin(), objList.end(),
				[&](sharedObj objA, sharedObj objB) {
				return LengthSquare(obj._pos, (*objA)._pos) < LengthSquare(obj._pos, (*objB)._pos);
			});

			for (auto list : objList)
			{
				// �͈͓��̓G�S�Ă���Ұ�ނ�^������
				if ((*list).teamTag() == TEAM_TAG::ALLY_TEAM)
				{
					if ((*list)._state != STATE::HOLDEN )
					{
						if ((obj._attackRange * obj._attackRange) > LengthSquare(obj._pos, (*list)._pos))
						{
							(*list).DoDamage(obj._power);		// ��Ұ��
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else
		{
			obj._coolCnt--;								// ������

			// �I��
			if (obj._coolCnt <= 0)
			{
				StopEffekseer2DEffect(obj._effectID);
				obj._effectFlg = false;
				obj._coolCnt = obj._coolCntMax;
				_move = MOVE_TYPE::CHARGE;
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
	lpEffectMng.AddEffectQue({ obj,obj._pos,0,EFFECT::FIRE,obj._zOrder + 1 });
}
