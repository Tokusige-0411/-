#include <algorithm>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "SceneMng.h"
#include <_DebugDispOut.h>
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "NetWork/NetWork.h"
#include "../common/EffectMng.h"
#include "../Map.h"
#include <UI_Mng.h>

std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::sInstance(new SceneMng());

void SceneMng::Run(void)
{
	SysInit();
	
	_activeScene = std::make_unique<TitleScene>();
	_dbgStartFPS();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameEndFlag() == false)
	{
		_dbgStartDraw();
		_drawList.clear();
		_soundList.clear();
		lpEffectMng.Clear();

		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		(*_activeScene).RunInstanceQue(std::move(_instanceList));
		(*_activeScene).RunActQue(std::move(_actList));
		lpEffectMng.Draw();
		Draw();
		SoundPlay();
		_frameCnt++;
	}
}

bool SceneMng::AddDrawQue(DrawQueT dQue)
{
	if (std::get<static_cast<int>(DRAW_QUE::IMAGE)>(dQue) <= 0)
	{
		// �摜ID���s���Ȃ̂Œǉ����Ȃ�
		return false;
	}
	// Que��ǉ�
	_drawList.emplace_back(dQue);
	return true;
}


bool SceneMng::AddActQue(ActQueT aQue)
{
	_actList.emplace_back(aQue);
	return true;
}

bool SceneMng::AddSoundQue(SoundQueT sQue)
{
	int id;
	double x, y;
	std::tie(id, std::ignore, x, y) = sQue;

	if ((x <= lpCamera.pos().x - lpCamera.size().x) &&
		(x >= lpCamera.pos().x + lpCamera.size().x) &&
		(y <= lpCamera.pos().y - lpCamera.size().y) &&
		(y >= lpCamera.pos().y + lpCamera.size().y)
		)
	{
		return false;
	}

	if (std::get<static_cast<int>(DRAW_QUE::IMAGE)>(sQue) <= 0)
	{
		// SoundID���s���Ȃ̂Œǉ����Ȃ�
		return false;
	}

	// Que��ǉ�
	_soundList.emplace_back(sQue);
	return true;
}

bool SceneMng::AddPlayerQue(PlayerQueT pQue)
{
	_playerList.emplace_back(pQue);
	return true;
}

bool SceneMng::AddInstanceQue(InstanceQueT iQue)
{
	UNIT_ID id;
	int type, num;
	Vector2Dbl pos;

	std::tie(id, type, pos, num) = iQue;

	num = _serialNumCnt;
	_serialNumCnt++;

	_instanceList.emplace(_instanceList.begin(), InstanceQueT{id,type,pos,num});
	return false;
}

const size_t SceneMng::GetPlayerListSize(void) const
{
	return _playerList.size();
}

std::vector<PlayerQueT>& SceneMng::playerList(void)
{
	return _playerList;
}

void SceneMng::FrameCntReset(void)
{
	_frameCnt = 0;
}

const int SceneMng::frameCnt(void) const
{
	return _frameCnt;
}

int SceneMng::serialNumCnt(void)
{
	return _serialNumCnt;
}

void SceneMng::AddSerialNum(void)
{
	_serialNumCnt++;
}

void SceneMng::SetShakeCount(int cnt)
{
	_shakeCount = cnt;
}

void SceneMng::SetGameClearFlag(bool flag)
{
	_gameClearFlag = flag;
	return;
}

bool & SceneMng::GameClearFlag(void)
{
	return _gameClearFlag;
}

void SceneMng::SetGameEndFlag(bool flag)
{
	_gameEndFlag = flag;
	return;
}

bool & SceneMng::GameEndFlag(void)
{
	return _gameEndFlag;
}

bool SceneMng::SetInstanceQue(InstanceQueT iQue)
{
	_instanceList.emplace_back(iQue);
	return true;
}

bool SceneMng::SysInit(void)
{
	// --------- �V�X�e������
	SetWindowText("Alchemy");

	SetGraphMode(static_cast<int>(ScreenSize.x), static_cast<int>(ScreenSize.y), 16);
	ChangeWindowMode(true);

	SetUseDirect3DVersion(DX_DIRECT3D_11);					// DIRECTX11���g����悤�ɂ���

	if (DxLib_Init() == -1)
	{
		return false;
	}

	if (Effekseer_Init(6000) == -1)
	{
		DxLib_End();
		return false;
	}

	// ��ذ݂̐؂�ւ���ؿ����������̂�h���B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXײ���؂����޲�۽Ă������̺���ޯ���ݒ肷��
	// ����޳���ٽ�ذ݂̐؂�ւ�����������ꍇ�ɂ͕K�����s����
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// �`��X�N���[���쐬
	_layerGID = MakeScreen(static_cast<int>(WorldSize.x), static_cast<int>(WorldSize.y), true);
	SetDrawScreen(DX_SCREEN_BACK);

	// Effekseer�ɕ`��̐ݒ������
	Effekseer_Set2DSetting(static_cast<int>(500), static_cast<int>(500));
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	_dbgSetup(255);

	// UI�X�N���[���̏�����
	lpUI_Mng.UI_Init();

	_frameCnt = 0;
	_gameClearFlag = false;
	_gameEndFlag = false;
	return false;
}

SceneMng::SceneMng() :ScreenSize{ 1280.0,720.0 }, ScreenCenter(ScreenSize / 2.0), WorldSize{ 2560.0,1440.0 }, WorldCenter(WorldSize / 2.0)
{
}

SceneMng::~SceneMng()
{
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();	// ���ޯ�ނ̷���𓊂���

	// DrawؽĂ̿��
	std::sort(_drawList.begin(), _drawList.end(),
		[](DrawQueT queA, DrawQueT queB) {
		return std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queA), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queA), std::get<static_cast<int>(DRAW_QUE::Y)>(queA)) <
			std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queB), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queB), std::get<static_cast<int>(DRAW_QUE::Y)>(queB));
	});


	// �`��X�N���[����������
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	LAYER drawLayer = begin(LAYER());
	int blendMode = DX_BLENDMODE_NOBLEND;
	int blendModeNum = 255;

	SetDrawScreen(_layerGID);
	SetDrawBlendMode(blendMode, blendModeNum);
	ClsDrawScreen();

	// ���܂��Ă���DrawQue�����ꂼ��̃��C���[�ɕ`�悷��
	for (auto que : _drawList)
	{
		double x, y, rad,height;
		int id;
		LAYER layer;

		double rate;		// �摜�̊g�嗦
		int blendModeOld = blendMode;
		int blendModeNumOld = blendModeNum;

		std::tie(id, x, y, rad, rate, height,std::ignore, layer, blendMode, blendModeNum) = que;

		/*if ((layer != drawLayer) || (blendMode != blendModeOld) || (blendModeNum != blendModeNumOld))
		{
			SetDrawScreen(DX_SCREEN_BACK);
			SetDrawBlendMode(blendModeOld, blendModeNumOld);
			auto layPos = ScreenCenter + (*_activeScene)._screenPos;
			DrawRotaGraph(static_cast<int>(layPos.x), static_cast<int>(layPos.y), 1.0, 0, _layerGID, true);

			SetDrawScreen(_layerGID);
			SetDrawBlendMode(blendMode, blendModeNum);
			ClsDrawScreen();
		}*/

		DrawRotaGraph(
			static_cast<int>(x),
			static_cast<int>(y) - static_cast<int>(height),
			rate,
			rad,
			id,
			true
		);
	}
	// ��ʂ�h�炷
	if (_shakeCount)
	{
		_shakeCount--;
		_screenPos = { static_cast<double>(rand() % 20 - 10) ,static_cast<double>(rand() % 20 - 10) };
	}
	if (!_shakeCount)
	{
		_screenPos = { 0,0 };
	}

	lpCamera.Draw(_layerGID);
	

	
	DrawRotaGraph(static_cast<int>(ScreenCenter.x), static_cast<int>(ScreenCenter.y), 1.0, 0, lpUI_Mng.screen(), true);

	//_dbgDrawFPS();
	ScreenFlip();
}

void SceneMng::SoundPlay(void)
{
	int id, vol;
	// ����ɗ��܂��Ă��鉹���Đ�����
	for (auto que : _soundList)
	{
		std::tie(id, vol, std::ignore, std::ignore) = que;
		ChangeVolumeSoundMem(vol, id);
		PlaySoundMem(id, DX_PLAYTYPE_BACK);
	}
}
