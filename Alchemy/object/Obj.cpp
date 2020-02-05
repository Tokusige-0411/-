#include <DxLib.h>
#include <Scene\SceneMng.h>
#include "Obj.h"
#include <_DebugConOut.h>
#include "Camera.h"

int Obj::_hpBarImg[2] = {0,0};
int Obj::_shadowImg = 0;

Obj::Obj()
{
	_alive = true;
	_dead = false;
	_holdFlag = false;
	_animFrame = 0;
	_animCount = 0;
	_speed = 0.0;
	_size = { 100,100 };
	_extRate = 1.2;
	_rad = 0.0;
	_zOrder = 0;
	_effectID = -1;
	_height = 0;
	_holdFlag = false;
	_effectFlg = false;
	_resultFlg = false;
	_damageCnt = 0;
	_hpMax = 0;
	_hp = 0;
	_hpID = MakeScreen(64, 8, false);
	_coolCnt = 0;
	_state = STATE::NORMAL;
	_dir = DIR::DOWN;
}

void Obj::Draw(void)
{
	// ---------- �͈̓`�F�b�N
	if (_animMap.find({ _state ,_dir}) == _animMap.end())
	{
		AST();
		return;
	}
	
	if (_animFrame >= _animMap[{ _state, _dir}].size() || _animFrame < 0)
	{
		AST();
		return;
	}

	// �A�j���[�V�����̃R�}���̍X�V�m�F
	if (_animCount >= _animMap[{ _state, _dir}][_animFrame].second)
	{
		// �����V���b�g�̊m�F
		if (_animMap[{ _state, _dir}][_animFrame].first != -1)
		{
			_animFrame++;
		}
	}

	// �A�j���[�V�������[�v�̊m�F
	if (_animFrame < _animMap[{ _state, _dir}].size())
	{
		_animCount++;
	}
	else
	{
		_animFrame = 0;
		_animCount = 0;
	}

	//// ���݂̃R�}�̉摜��`��L���[�ɒǉ�

	/*if ((_pos.x >= lpCamera.pos().x - lpCamera.size().x - _size.x) &&
		(_pos.x <= lpCamera.pos().x + lpCamera.size().x + _size.x) &&
		(_pos.y >= lpCamera.pos().y - lpCamera.size().y - _size.y) &&
		(_pos.y <= lpCamera.pos().y + lpCamera.size().y + _size.y + _height)
		)*/
	{
		// ���݂̃R�}�̉摜��`��L���[�ɒǉ�
		lpSceneMng.AddDrawQue({ _animMap[{ _state ,_dir}][_animFrame].first,
			_pos.x,
			_pos.y,
			0.0,
			_extRate + (_height /400.0),
			_height,
			_zOrder,
			LAYER::CHAR ,
			DX_BLENDMODE_NOBLEND,
			255 });

		// HP�o�[
		DrawHP();
	}

	// �G�t�F�N�g��������
	if (_glowFlag)
	{
		SetDrawScreen(_glowID);
		ClsDrawScreen();
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(_size.x, _size.y, 1.1, 0, _animMap[{_state, _dir}][_animFrame].first, true);
		SetDrawBright(255, 255, 255);
		GraphFilter(_glowID, DX_GRAPH_FILTER_GAUSS, 8, 100);
		GraphFilter(_glowID, DX_GRAPH_FILTER_TWO_COLOR, 10, 0, 0, 0xffffff, 255);
		lpSceneMng.AddDrawQue({ _glowID,_pos.x,
			_pos.y - 0.1,
			0.0,_extRate,_height,_zOrder,LAYER::CHAR,DX_BLENDMODE_ADD,255 });

		_glowFlag = false;
	}
	else
	{
		// �e���`��
		// �����ɂ���đ傫����ς����肷�邩��
		if (static_cast<int>(_state) < static_cast<int>(STATE::HOLDEN))
		{
			lpSceneMng.AddDrawQue({ _shadowImg,
			_pos.x,
			_pos.y + _size.y / 2.0,
			0.0,
			_extRate - (_height / 5000.0),
			0.0,
			INT_MIN,
			LAYER::CHAR ,
			DX_BLENDMODE_NOBLEND,
			255 });
		}
	}

}

void Obj::Draw(int id)
{
	lpSceneMng.AddDrawQue({ id,
							_pos.x,
					        _pos.y,
					        _rad,
							1.2,
							_height,
							_zOrder,
							LAYER::CHAR, 
							DX_BLENDMODE_NOBLEND, 
							255
	});
}


Obj::~Obj()
{
}

// STATE��get,set
// get
STATE Obj::state(void) const
{
	return _state;
}

// set
bool Obj::state(STATE newState)
{
	if (_animMap.find({ newState,_dir }) == _animMap.end())
	{
		// �o�^�����A�j���[�V�����̃L�[�̒��ɑ��݂��Ȃ�
		AST();
		return false;
	}

	// ���݂Ɠ����A�j���[�V�����̎��̓R�}���ƃJ�E���g���N���A���Ȃ�
	if (_state != newState)
	{
		_animFrame = 0;
		_animCount = 0;
	}

	_state = newState;

	return true;
}

DIR Obj::dir(void)const
{
	return _dir;
}

bool Obj::dir(DIR newDir)
{
	if (_animMap.find({ _state,newDir }) == _animMap.end())
	{
		// �o�^�����A�j���[�V�����̃L�[�̒��ɑ��݂��Ȃ�
		AST();
		return false;
	}

	// ���݂Ɠ����A�j���[�V�����̎��̓R�}���ƃJ�E���g���N���A���Ȃ�
	if (_dir != newDir)
	{
		_animFrame = 0;
		_animCount = 0;
	}

	_dir = newDir;

	return true;
}

bool Obj::alive(bool alive)
{
	_alive = alive;
	if (!_alive)
	{
		_zOrder = 10;
		state(STATE::DEATH);
	}
	return true;
}

// _dead�̓����o���炵���ύX���Ȃ����炢��Ȃ�����
// _alive��set���g���Ă邩�炻��ɍ��킹�Ĉꉞ
bool Obj::dead(bool dead)
{
	if (alive())
	{
		// ���󔚔����I�������������g��Ȃ��̂Ő������Ă���Ƃ��͒l��ݒ肵�Ȃ�
		return false;
	}

	_dead = dead;

	return true;
}

const Vector2Dbl& Obj::pos(void)
{
	return _pos;
}

const Vector2Int & Obj::size(void)
{
	return _size;
}

const double & Obj::rad(void)
{
	return _rad;
}

void Obj::exFlag(bool isEX)
{
	_exFlag = isEX;

	return;
}

int Obj::getType(void)
{
	return 0;
}

void Obj::glowFlag(bool isGlow)
{
	_glowFlag = isGlow;
}

const int Obj::weight(void)
{
	return _weight;
}

const UNIT_ID & Obj::unitID(void)
{
	return _unitID;
}

const TEAM_TAG & Obj::teamTag(void)
{
	return _team;
}

std::weak_ptr<Obj>& Obj::tageObj(void)
{
	// �Ώۂ�obj��Ԃ�
	return _tageObj;
}

void Obj::SetTageObj(sharedObj obj)
{
	_tageObj = obj;
}

const double & Obj::searchRange(void)
{
	return _searchRange;
}

const double & Obj::attackRange(void)
{
	return _attackRange;
}

void Obj::DoDamage(int power)
{
	_hp -= power;
	if (_hp > _hpMax)
	{
		_hp = _hpMax;
	}
	_damageCnt = 180;
}

const int & Obj::hp(void)
{
	return _hp;
}

const int & Obj::hpMax(void)
{
	return _hpMax;
}

bool Obj::isAnimEnd(void)
{
	// ---------- �͈̓`�F�b�N
	if (_animMap.find({ _state ,_dir }) == _animMap.end())
	{
		AST();
		return true;
	}

	if (_animFrame >= _animMap[{ _state, _dir}].size() || _animFrame < 0)
	{
		AST();
		return true;
	}

	if (_animMap[{ _state, _dir}][_animFrame].first == -1)
	{
		return true;
	}

	return false;
}

// �A�j���[�V�����̃f�[�^��o�^
bool Obj::SetAnim(const AnimKey key, AnimVector& data)
{
	return _animMap.try_emplace(key, std::move(data)).second;

	/*if (_animMap.find(key) == _animMap.end())
	{
		_animMap[key] = std::move(data);
		return true;
	}
	
	return false;*/
}

// �I�u�W�F�ŋ��ʂŎg���摜��ǂݍ���
// GameScene��Init�Ś�I�u�W�F����Ăяo��
void Obj::LoadStaticImg(void)
{
	_hpBarImg[0] = lpImageMng.GetID({ IMG::HP_R ,STATE::NORMAL }, "image/unitHP_R.png")[0];
	_hpBarImg[1] = lpImageMng.GetID({ IMG::HP_G ,STATE::NORMAL }, "image/unitHP_G.png")[0];

	_shadowImg = lpImageMng.GetID({ IMG::SHADOW,STATE::NORMAL }, "image/shadow.png")[0];
}

const bool Obj::resultFlg(void)
{
	return _resultFlg;
}

void Obj::resultFlg(bool flag)
{
	_resultFlg = flag;
}

void Obj::SetresultFlg(bool id)
{
	_resultFlg = id;
}

// ������Ԋm�F,���S��Ԑݒ�
bool Obj::DestroyProc(void)
{
	if (alive())
	{
		return false;
	}
	else
	{
		if (_state != STATE::DEATH)
		{
			state(STATE::DEATH);
		}
	}


	if (isAnimEnd())
	{
		dead(true);
	}

	return true;
}

void Obj::DrawHP(void)
{
	// �܂����ɐԂ��o�[��`�悵�A��ɗ΂̃o�[��RectGraph�Ŏc��HP�ɏ]���ĕ`��
	if (_damageCnt > 0)
	{
		if (_hpMax != 0)
		{
			SetDrawScreen(_hpID);
			DrawGraph(0, 0, _hpBarImg[0], false);
			DrawRectGraph(0, 0, 0, 0, 64 * _hp / _hpMax, 8, _hpBarImg[1], false, false);
			SetDrawScreen(DX_SCREEN_BACK);

			lpSceneMng.AddDrawQue({ _hpID,
				_pos.x,
				_pos.y - 48,
				0.0,
				1.0,
				_height,
				_zOrder + 1,
				LAYER::UI ,
				DX_BLENDMODE_NOBLEND,
				255 });
			_damageCnt--;
		}
	}
}
