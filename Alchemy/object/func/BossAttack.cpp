#include "BossAttack.h"
#include <DxLib.h>
#include <algorithm>
#include <_DebugConOut.h>
#include <EffekseerForDXLib.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>

void BossAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
{
	// �����N�؂�
	if (obj.tageObj().expired())
	{
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
		if (obj._hp <= (obj._hpMax / 2))
		{
			_type = EFFECT::FIRE_TWO;
		}
		else
		{
			_type = EFFECT::DARK;
		}
		Blast(obj, objList);
		break;
	default:
		AST();
		break;
	}


}

void BossAttack::Charge(Obj & obj)
{
	auto target = obj.tageObj().lock();					// �^�[�Q�b�g�̏��i�[

	// �Đ�����
	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		if (!obj._effectFlg)
		{
			_chargeCnt++;
			if (_chargeCnt <= 1)
			{
				_effectPos = (*target)._pos;
			}
		}
		else
		{
			// 2��x���o�����玟�̍U���Ɉړ�
			if (_chargeCnt >= 3)
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
	lpEffectMng.AddEffectQue({ obj,_effectPos,0,EFFECT::BOSS_WAR,obj._zOrder + 1 });
}

void BossAttack::Blast(Obj & obj, std::vector<sharedObj>& objList)
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
				return LengthSquare(_effectPos, (*objA)._pos) < LengthSquare(_effectPos, (*objB)._pos);
			});

			for (auto list : objList)
			{
				// �͈͓��̓G�S�Ă���Ұ�ނ�^������
				if ((*list).teamTag() == TEAM_TAG::ALLY_TEAM)
				{
					if ((*list)._state != STATE::HOLDEN)
					{
						if ((obj._attackRange * obj._attackRange) > LengthSquare(_effectPos, (*list)._pos))
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
	lpEffectMng.AddEffectQue({ obj,_effectPos,0,_type,obj._zOrder + 1 });
}
