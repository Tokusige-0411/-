#include "GhostAttack.h"
#include <cmath>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <_DebugConOut.h>
#include <scene\SceneMng.h>
#include <EffectMng.h>
#include <EFFECT.h>

void GhostAttack::operator()(Obj & obj, std::vector<sharedObj>& objList)
{
	// �����N�؂�
	if (obj.tageObj().expired())
	{
		StopEffekseer2DEffect(obj._effectID);
		obj._effectFlg = false;
		_move = MOVE_TYPE::CHARGE;
		obj.state(STATE::NORMAL);
		return;
	}
	
	switch (_move)
	{
	case MOVE_TYPE::CHARGE:
		Charge(obj);
		break;
	case MOVE_TYPE::SHOT:
		Shot(obj);
		break;
	case MOVE_TYPE::BLAST:
		Blast(obj);
		break;
	default:
		AST();
		break;
	}


}

void GhostAttack::Charge(Obj & obj)
{
	auto target = obj.tageObj().lock();								// �^�[�Q�b�g�̏��i�[

	// ���ĂŔ���
	if (_endCnt >= 75)
	{
		StopEffekseer2DEffect(obj._effectID);
		_endCnt = 0;
		_shotPos = obj._pos;										// �e�̏������W
		_move = MOVE_TYPE::SHOT;
		return;
	}

	if (IsEffekseer2DEffectPlaying(obj._effectID) == 0)
	{
		_endCnt++;
	}

	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj, obj._pos,0, EFFECT::GHOST_CHARGE, obj._zOrder + 1});
}

void GhostAttack::Shot(Obj & obj)
{
	auto target = obj.tageObj().lock();								// ���ޯĂ̏��i�[

	if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
	{
		_shotPos = obj._pos;										// �e�̏������W
	}
	else
	{
		// ���������甚�����Ăق���
		if (IsHitObj(_shotPos, (*target)._pos, (*target)._size))
		{
			StopEffekseer2DEffect(obj._effectID);
			_endCnt = 0;
			_move = MOVE_TYPE::BLAST;
			return;
		}

		_shotPos += ((*target)._pos - obj._pos) / 30.0;				// �ړ�
		_endCnt++;
	}

	// �U�����f
	if (_endCnt >= 60)
	{
		StopEffekseer2DEffect(obj._effectID);
		_move = MOVE_TYPE::CHARGE;
		_endCnt = 0;
		obj.state(STATE::NORMAL);
		return;
	}

	// �̪�Ă𷭰�ɓ�����
	lpEffectMng.AddEffectQue({ obj, _shotPos, 0, EFFECT::GHOST_SHOT, (*target)._zOrder + 1 });
}

void GhostAttack::Blast(Obj & obj)
{
	auto target = obj.tageObj().lock();								// ���ޯĂ̏��i�[

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
				obj._effectFlg = false;
				_move = MOVE_TYPE::CHARGE;
				obj._coolCnt = obj._coolCntMax;
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
	lpEffectMng.AddEffectQue({ obj, (*target)._pos, 0, EFFECT::GHOST_BLAST, (*target)._zOrder + 1 });
}

bool GhostAttack::IsHitObj(Vector2Dbl shot, Vector2Dbl targetPos, Vector2Int targetSize)
{
	Vector2Dbl lenght = { targetPos.x - shot.x, targetPos.y - shot.y };

	if (sqrt(lenght.x * lenght.x + lenght.y * lenght.y) < sqrt(targetSize.x) &&
		sqrt(lenght.x * lenght.x + lenght.y * lenght.y) < sqrt(targetSize.y))
	{
		return true;
	}

	return false;
}
