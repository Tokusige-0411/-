#include "ThrownUpdate.h"
#include <scene/SceneMng.h>
#include <EffekseerForDXLib.h>
#include <EffectMng.h>
#include <Camera.h>
#include "../Pot.h"
#include "../Enemy.h"

void ThrownUpdate::operator()(Obj& obj, std::vector<sharedObj>& objList)
{
	// ��ʊO����
	auto tmpPos = obj._pos + obj._vel.x * Vector2Dbl{ cos(obj._rad), sin(obj._rad) };
	//tmpPos.y -= obj._height;

	// x���Ŕ��]
	if (tmpPos.x <= 0 || tmpPos.x >= lpSceneMng.WorldSize.x)
	{
		obj._rad = atan2(sin(obj._rad), -cos(obj._rad));
	}
	
	// y���Ŕ��]
	if (tmpPos.y /*- obj._height*/ <= 0 || tmpPos.y >= lpSceneMng.WorldSize.y - static_cast<double>(obj._size.y) / 2.0)
	{
		obj._rad = atan2(-sin(obj._rad), cos(obj._rad));
	}

	obj._pos += obj._vel.x * Vector2Dbl{cos(obj._rad), sin(obj._rad)};
	obj._height += obj._vel.y;

	// ���G�t�F�N�g���Ă݂�
	/*lpSceneMng.AddEffectQue({ EFFECT_ID(EFFECT::DASH),
								 obj._pos.x - lpCamera.OfSet().x,
								obj._pos.y - lpCamera.OfSet().y - obj._height,
								100,LAYER::CHAR });*/

	// �������I������
	if (obj._height <= 0)
	{
		// ��Ɍ������Ă���Ƃ��͚�ɓ��邩���肷��
		if (!obj._tageObj.expired())
		{
			if ((*obj._tageObj.lock()).unitID() == UNIT_ID::POT)
			{
				lpSceneMng.AddActQue({ ACT_QUE::INTO_POT, obj });
				return;
			}
		}
		// ���n�̪�Ă��Đ�������
		if (IsEffekseer2DEffectPlaying(obj._effectID) == -1)
		{
			if (obj._effectFlg)
			{
				// �X�e�[�^�X��߂�
				StopEffekseer2DEffect(obj._effectID);
				obj._effectFlg = false;
				obj._height = 0;
				if (obj._unitID == UNIT_ID::PLAYER)
				{
					// Player����������Ƃ��͎���ł鎞�ƕ������Ě₩��o�Ă���Ƃ�
					if (obj._hp <= 0)
					{
						obj.dir(DIR::DOWN);
						obj.state(STATE::DEATH);
						// Draw�̃^�C�~���O�̉e���ł����ŉe��\�����Ȃ��ƈ�u������
						lpSceneMng.AddDrawQue({ obj._shadowImg,
												obj._pos.x,
												obj._pos.y + obj._size.y / 2.0,
												0.0,
												obj._extRate - (obj._height / 5000.0),
												0.0,
												INT_MIN,
												LAYER::CHAR ,
												DX_BLENDMODE_NOBLEND,
												255 });
					}
					else
					{
						obj.state(STATE::NORMAL);
					}
					
				}
				else
				{
					obj.state(STATE::NORMAL);
				}
				obj._tageObj.reset();
				return;
			}
		}
		else
		{
			obj._effectFlg = true;
		}
		Vector2Dbl ofSet = { 0.0,static_cast<double>(obj._size.y / 2) };
		// �̪�Ă𷭰�ɓ�����
		lpEffectMng.AddEffectQue({ obj,obj._pos + ofSet,0,EFFECT::LANDING,obj._zOrder + 1 });
		obj._vel = { 0,0 };
		obj._height = 0;
		return;
	}

	obj._vel.y -= obj._gravity;
}
