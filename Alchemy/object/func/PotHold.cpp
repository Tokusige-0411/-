#include <DxLib.h>
#include <_DebugConOut.h>
#include "PotHold.h"
#include "../Pot.h"
#include <Camera.h>
#include "../FOLLOWER_TYPE.h"

void PotHold::operator()(Obj& pot, std::vector<sharedObj>& objList)
{
	if (((Pot&)pot)._mixList.empty())
	{
		AST();
		pot.state(STATE::NORMAL);
		return;
	}

	// �R���{�Q�[�W�\��
	auto rect = static_cast<int>(80.0 * static_cast<double>(((Pot&)pot)._comboCnt) / static_cast<double>(COMBO_TIME_MAX - 30 * (((Pot&)pot)._comboNum - 1)));
	SetDrawScreen(((Pot&)pot)._gaugeID);
	ClsDrawScreen();
	DrawGraph(0,0, ((Pot&)pot)._gaugeImg[0], true);
	DrawRectGraph(5, 5, 0, 0, rect, 16, ((Pot&)pot)._gaugeImg[1], true);
	SetDrawScreen(DX_SCREEN_BACK);
	lpSceneMng.AddDrawQue({ ((Pot&)pot)._gaugeID,
		pot._pos.x,
		pot._pos.y + 60.0,
		0.0,
		1.0,
		0.0,
		0,
		LAYER::UI ,
		DX_BLENDMODE_NOBLEND,
		255 });

	// �����X�^�[���o��
	// �R���{�̎��Ԃ��؂��or�R���{����ɒB����
	if (--((Pot&)pot)._comboCnt <= 0 || ((Pot&)pot)._comboNum >= COMBO_NUM_MAX)
	{
		char combiBit = 0;
		int num = 0;		// �R���{�ɂ���đ�����r�o��

		// �����Ă������̂�����
		// Follower�������Ă��炻�̂܂܏o��
		for (auto mixData : ((Pot&)pot)._mixList)
		{
			UNIT_ID checkID;
			int type;
			std::tie(checkID, type) = mixData;
			if (checkID == UNIT_ID::FOLLOWER)
			{
				((Pot&)pot)._throwList.emplace_back(lpSceneMng.serialNumCnt());
				lpSceneMng.AddInstanceQue({ UNIT_ID::FOLLOWER, type,pot._pos,0 });
			}
			else if (checkID == UNIT_ID::ITEM)
			{
				// �g�ݍ��킹�̃r�b�g�}�b�v�쐬
				combiBit = combiBit | type;
			}
		}

		// �r�o������
		// ���͊m��ł�������i = 2����n�܂�
		for (int i = 2; i < ((Pot&)pot)._comboNum + 1; i++)
		{
			num = i*2-1;
		}

		if (combiBit)
		{

			// �m��̂������
			((Pot&)pot)._throwList.emplace_back(lpSceneMng.serialNumCnt());
			lpSceneMng.AddInstanceQue({ UNIT_ID::FOLLOWER, ((Pot&)pot)._combiList[combiBit - 1].confType,pot._pos,0 });

			// �z������
			for (int i = 0; i < num; i++)
			{
				int type = ((Pot&)pot)._combiList[combiBit - 1].mixProb[rand() % 10];		// �����ŏo�Ă���̂�0�`4�Ȃ̂œ�i���̃r�b�g���ɕϊ�
				type = static_cast<int>(pow(2.0, type));
				((Pot&)pot)._throwList.emplace_back(lpSceneMng.serialNumCnt());
				lpSceneMng.AddInstanceQue({ UNIT_ID::FOLLOWER, type,pot._pos,0 });
			}
		}
		lpSceneMng.AddActQue({ ACT_QUE::THOROW , pot });
		((Pot&)pot)._mixList.clear();
		((Pot&)pot)._comboNum = 0;

		return;
	}
}

