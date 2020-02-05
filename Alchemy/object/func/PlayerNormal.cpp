#include <DxLib.h>
#include <scene/SceneMng.h>
#include "PlayerNormal.h"
#include "../Player.h"
#include <Input/InputState.h>
#include <UI_Mng.h>
#include <Camera.h>

void PlayerNormal::operator()(Obj& player, std::vector<sharedObj>& objList)
{
	// ���������ĂȂ��Ƃ�
		// ���㎝�Ă鐔��������\��������̂ŏ������ς�邩��

	// �^�[�Q�b�g����
	for (auto obj : objList)
	{
		if (LengthSquare((*obj).pos(), player._pos) > player._searchRange * player._searchRange)
		{
			// ��Ń\�[�g���Ă�̂ō��G�͈͂𒴂����G�������画��I���
			break;
		}

		// holden,thrwon,death�̎��͎��ĂȂ�
		if ((*obj).weight() > ((Player&)player).holdWeightMax())
		{
			continue;
		}

		if ((*obj)._unitID != UNIT_ID::PLAYER && static_cast<int>((*obj).state()) >= static_cast<int>(STATE::THROWN))
		{
			continue;
		}

		player._tageObj = obj;
		((Player&)player).MakeBtnGuide(INPUT_ID::BTN_B);
		break;
	}

	// ����
	if (!(player._tageObj.expired()) && (((*player._input).btnState(INPUT_ID::BTN_B).first && !(*player._input).btnState(INPUT_ID::BTN_B).second)))
	{
		lpSceneMng.AddActQue({ ACT_QUE::HOLD,player });
		lpSceneMng.AddSoundQue({ lpSoundMng.GetID(SOUND::HOLD)[0], 255 , player.pos().x, player.pos().y });
	}
}

