#include <algorithm>
#include <_DebugConOut.h>
#include "Player.h"
#include <Input/PadState.h>
#include <Input/NetState.h>
#include <Scene/SceneMng.h>
#include <Camera.h>
#include <Map.h>
#include "NetWork/NetWork.h"
#include "func/PlayerNormal.h"
#include "func/PlayerHold.h"
#include "func/PlayerThrow.h"
#include "func/PlayerDeath.h"
#include "func/ThrownUpdate.h"
#include "func/HoldenUpdate.h"
#include "Pot.h"
#include <UI_Mng.h>

int Player::_playerIcon[4][2];
int Player::pCnt = 0;
bool Player::_menuFlag[2] = { false ,false};

Player::Player()
{
	_input = std::make_unique<PadState>(DX_INPUT_PAD1);
	_plNum = PlNum::PL_01;
	_pos = {1080.0,800.0};
	_size = { 60,64 };

	Init();
}

Player::Player(PlNum plNum, Vector2Dbl pos, int padNum,sharedObj potObj)
{
	_input = std::make_unique<PadState>(padNum);
	_plNum = plNum;
	_pos = pos;
	_potObj = potObj;
	_size = { 60,64 };
	_checkNum = pCnt;
	pCnt++;

	Init();
}

Player::Player(PlNum plNum, Vector2Dbl pos, sharedObj potObj)
{
	_input = std::make_unique<NetState>(plNum);
	_plNum = plNum;
	_pos = pos;
	_potObj = potObj;
	_size = { 60,64 };

	Init();
}

void Player::Update(std::vector<sharedObj>& objList)
{
	(*_input).Update(objList);
	if (state() != STATE::THROWN)
	{
		_tageObj.reset();
	}

	// 自分からの距離が近い順にobjListをソート
	std::sort(objList.begin(), objList.end(),
		[&](sharedObj objA, sharedObj objB) {
		return LengthSquare((*objA).pos(), _pos) < LengthSquare((*objB).pos(), _pos);
	});

	//　現段階では死なないほうがデバックしやすいので
	if (static_cast<int>(state()) < static_cast<int>(STATE::ATTACK))
	{
		if (_hp <= 0)
		{
			dir(DIR::DOWN);
			state(STATE::DEATH);
			_weight = 1;
		}
	}

	if (_menuFlag[_checkNum])
	{
		Menu();
		
	}
	else if(static_cast<int>(state()) < static_cast<int>(STATE::ATTACK))
	{
		if ((*_input).LStickState().first.isInput)
		{
			_rad = RAD(static_cast<double>((*_input).LStickState().first.angle));
			auto move = _speed * Vector2Dbl{ cos(_rad),sin(_rad) };			// 移動距離

			// 移動制限
			// 画面横
			if (_pos.x + move.x <= _size.x / 2.0 || _pos.x + move.x >= lpSceneMng.WorldSize.x - _size.x / 2.0)
			{
				move.x = 0;
			}
			// 画面上下
			if (_pos.y + move.y <= _size.y / 2.0 || _pos.y + move.y >= lpSceneMng.WorldSize.y - _size.y / 2.0)
			{
				move.y = 0;
			}
			_pos += move;

			dir((*_input).LStickState().first.dir);
		}

		// ミニマップの表示切替
		if ((*_input).btnState(INPUT_ID::BTN_START).first && !(*_input).btnState(INPUT_ID::BTN_START).second)
		{
			lpMap.miniMapDrawFlag();
		}

		// テスト用のカメラ特殊移動,あとでけす
		if ((*_input).btnState(INPUT_ID::BTN_RS).first && !(*_input).btnState(INPUT_ID::BTN_RS).second)
		{
			lpSceneMng.AddInstanceQue({ UNIT_ID::BOSS,0,{lpSceneMng.WorldSize.x / 2.0,0},0 });
			lpUI_Mng.FadeCnt(510);
			lpCamera.SetMoveData({ 1280.0,390.0 });
			lpCamera.exMoveFlag(true);
		}

		// メッセージテスト、あとでけす
		if ((*_input).btnState(INPUT_ID::UP).first && !(*_input).btnState(INPUT_ID::UP).second)
		{
			Text mes;
			mes.MakeText("こんにちは", -4);
			lpUI_Mng.AddMessage(mes);
		}

		if ((*_input).btnState(INPUT_ID::DOWN).first && !(*_input).btnState(INPUT_ID::DOWN).second)
		{
			Text mes;
			mes.MakeText("アイテムをもっています", -8);
			lpUI_Mng.AddMessage(mes);
		}

		// ﾒﾆｭｰ画面の表示
		if ((*_input).btnState(INPUT_ID::BTN_X).first && !(*_input).btnState(INPUT_ID::BTN_X).second)
		{
			if (!_menuFlag[_checkNum] && !_menuFlag[(_checkNum + 1) % 2])
			{
				_menuFlag[_checkNum] = true;

			}
			_cursorNum = 0;
		}
	}
	else
	{

	}

	if (!_menuFlag[_checkNum])
	{
		try
		{
			_funcState[state()](*this, objList);
		}
		catch (...)
		{
			AST();
		}
	}
	else
	{
		if ((*_input).btnState(INPUT_ID::BTN_A).first && !(*_input).btnState(INPUT_ID::BTN_A).second)
		{
			_menuFlag[_checkNum] = false;
		}
	}

	// targetが設定されているときはそいつを光らせる
	if (!_tageObj.expired())
	{
		(*_tageObj.lock()).glowFlag(true);
	}
	lpMap.ChangeChip(_pos, _rad, 1);

	// 画面を揺らす
	if (_shakeCount)
	{
		_shakeCount--;
		_shakeOffset = { static_cast<double>(rand() % 20 - 10) ,static_cast<double>(rand() % 20 - 10) };
	}
	if (!_shakeCount)
	{
		_shakeOffset = { 0,0 };
	}

	/*TRACE("%lf\n", _pos.x);
	TRACE("%lf\n", _pos.y);*/
	lpNetWork.SyncObj(*this);
	lpNetWork.MakeSyncMes(_pos);
}

PlNum Player::plNum(void)
{
	return _plNum;
}

int Player::GetpCnt(void)
{
	return pCnt;
}

void Player::ReSetpCnt(void)
{
	pCnt = 0;
}

int Player::holdWeightMax(void)
{
	return _holdWeightMax;
}

int Player::GetHoldListSize(void)
{
	return static_cast<int>(_holdList.size());
}

void Player::MakeBtnGuide(INPUT_ID btn)
{
	SetDrawScreen(_buttonID);
	ClsDrawScreen();

	int num = static_cast<int>(btn);
	if (num < static_cast<int>(INPUT_ID::BTN_LB))
	{
		AST();
		return;
	}
	else if (num < static_cast<int>(INPUT_ID::BTN_A))
	{
		num += 2;
	}
	else
	{

	}
	
	DrawGraph(0, 0, lpImageMng.GetID({ IMG::BUTTON,STATE::NORMAL })[(num - 10) * 2 + (lpSceneMng.frameCnt() / 30) % 2], true);
	SetDrawScreen(DX_SCREEN_BACK);

	lpSceneMng.AddDrawQue({ _buttonID,
			_pos.x,
			_pos.y,
			0.0,
			2.0,
			_height + 60,
			INT_MAX,
			LAYER::CHAR ,
			DX_BLENDMODE_NOBLEND,
			255 });
	return;
}

void Player::MakeBtnGuide(INPUT_ID btn,Vector2Dbl drawPos)
{
	SetDrawScreen(_buttonID);
	ClsDrawScreen();

	int num = static_cast<int>(btn);
	if (num < static_cast<int>(INPUT_ID::BTN_LB))
	{
		AST();
		return;
	}
	else if (num < static_cast<int>(INPUT_ID::BTN_A))
	{
		num += 2;
	}
	else
	{

	}

	DrawGraph(0, 0, lpImageMng.GetID({ IMG::BUTTON,STATE::NORMAL })[(num - 10) * 2 + (lpSceneMng.frameCnt() / 30) % 2], true);
	SetDrawScreen(DX_SCREEN_BACK);

	lpSceneMng.AddDrawQue({ _buttonID,
			drawPos.x,
			drawPos.y,
			0.0,
			2.0,
			0,
			INT_MAX,
			LAYER::UI ,
			DX_BLENDMODE_NOBLEND,
			255 });
	return;
}

bool Player::throwPot(void)
{
	return _throwPot;
}

const int Player::checkNum(void) const
{
	return _checkNum;
}


Player::~Player()
{
}

void Player::Init(void)
{
	// ----------アニメーション登録開始
	AnimVector data;

	ImageKey key = { static_cast<IMG>(_plNum),STATE::NORMAL };

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 10);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 20);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 40);

		SetAnim({ STATE::NORMAL,dir }, data);
	}

	// 投げ画像がまだないので一旦NORMAL
	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 10);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 20);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 40);

		SetAnim({ STATE::THROW,dir }, data);
	}

	key.second = STATE::HOLD;

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4], 10);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 1], 20);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 2], 30);
		data.emplace_back(IMAGE_ID(key)[static_cast<int>(dir) * 4 + 3], 40);

		SetAnim({ STATE::HOLD,dir }, data);
	}

	key.second = STATE::DEATH;

	data.emplace_back(IMAGE_ID(key)[0], 10);
	data.emplace_back(IMAGE_ID(key)[0], 10);

	SetAnim({ STATE::DEATH,DIR::DOWN }, data);

	data.emplace_back(IMAGE_ID(key)[0], 10);
	data.emplace_back(IMAGE_ID(key)[0], 10);

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[0], 10);
		data.emplace_back(IMAGE_ID(key)[0], 10);

		SetAnim({ STATE::THROWN,dir }, data);
	}

	for (auto dir = DIR::LEFT; dir != DIR::MAX; dir = static_cast<DIR>(static_cast<int>(dir) + 1))
	{
		data.emplace_back(IMAGE_ID(key)[0], 10);
		data.emplace_back(IMAGE_ID(key)[0], 10);

		SetAnim({ STATE::HOLDEN,dir }, data);
	}
	
	_unitID = UNIT_ID::PLAYER;
	_team = TEAM_TAG::ALLY_TEAM;
	_searchRange = 50.0;
	_weight = 10;
	_holdWeightMax = 4;
	_throwRange = 300.0;
	_serialNum = lpSceneMng.serialNumCnt();
	lpSceneMng.AddSerialNum();
	_playerHPImg[0] = lpImageMng.GetID({ IMG::PL_HP_R,STATE::NORMAL }, "image/playerHP_Back.png")[0];
	_playerHPImg[1] = lpImageMng.GetID({ IMG::PL_HP_G,STATE::NORMAL }, "image/playerHP_Frame.png")[0];
	_playerHPImg[2] = lpImageMng.GetID({ IMG::PL_HP_G,STATE::HOLD }, "image/playerHP_Gauge.png")[0];

	if (_playerIcon[0][0] == 0)
	{
		lpImageMng.GetID({ IMG::PL_ICON ,STATE::NORMAL }, "image/Player_icon.png", { 72,70 }, { 2,4 });
		for (auto num : PlNum())
		{
			_playerIcon[static_cast<int>(num)][0] = lpImageMng.GetID({ IMG::PL_ICON ,STATE::NORMAL })[static_cast<int>(num) * 2];
			_playerIcon[static_cast<int>(num)][1] = lpImageMng.GetID({ IMG::PL_ICON ,STATE::NORMAL })[static_cast<int>(num) * 2 + 1];
		}
	}

	_hpID = MakeScreen(384, 86, true);

	_speed = 5.0;
	_glowID = MakeScreen(_size.x * 2, _size.y * 2, true);
	_buttonID = MakeScreen(16, 16, true);

	_funcState = { {STATE::NORMAL,PlayerNormal()},{STATE::HOLD,PlayerHold()},{STATE::THROW,PlayerThrow()} ,{STATE::DEATH,PlayerDeath()} ,{STATE::THROWN,ThrownUpdate()},{STATE::HOLDEN,HoldenUpdate()} };

	_hpMax = 20;
	_hp = _hpMax;

	// 初期アニメーション
	state(STATE::NORMAL);
	dir(DIR::DOWN);
}

void Player::DrawHP(void)
{
	// 操作対象のプレイヤーだったらUIに表示、ほかのプレイヤーだったらObjのDrawを呼び出すようにする
	// 今はネットワークつないでないので呼び出さない
	SetDrawScreen(_hpID);
	DrawGraph(0, 0, _playerHPImg[0], true);
	DrawGraph(8, 8, _playerIcon[static_cast<int>(_plNum)][0], true);
	DrawRectGraph(92, 8, 0, 0, 284 * _hp / _hpMax, 40, _playerHPImg[2], true, false);
	DrawGraph(0, 0, _playerHPImg[1], true);

	SetDrawScreen(DX_SCREEN_BACK);

	lpUI_Mng.AddUIQue({_checkNum * lpSceneMng.ScreenCenter.x,16,_hpID});
}

void Player::Menu(void)
{
	if ((*_input).btnState(INPUT_ID::UP).first && !(*_input).btnState(INPUT_ID::UP).second)
	{
		if ((_cursorNum % 3))
		{
			_cursorNum--;
		}
		else
		{
			_cursorNum += 2;
		}
	}

	if ((*_input).btnState(INPUT_ID::DOWN).first && !(*_input).btnState(INPUT_ID::DOWN).second)
	{
		if ((_cursorNum % 3) == 2)
		{
			_cursorNum-= 2;
		}
		else
		{
			_cursorNum++;
		}
	}

	if (((*_input).btnState(INPUT_ID::LEFT).first && !(*_input).btnState(INPUT_ID::LEFT).second)
		|| ((*_input).btnState(INPUT_ID::RIGHT).first && !(*_input).btnState(INPUT_ID::RIGHT).second))
	{
		if ((_cursorNum / 3))
		{
			_cursorNum -= 3;
		}
		else
		{
			_cursorNum += 3;
		}
	}
	auto pot = _potObj.lock();
	std::vector<int>& followerList = ((Pot&)(*pot)).followerCnt();

	// 壺から出してインスタンス
	if ((*_input).btnState(INPUT_ID::BTN_B).first && !(*_input).btnState(INPUT_ID::BTN_B).second)
	{
		auto type = static_cast<int>(pow(2, _cursorNum));
		if (followerList[_cursorNum] > 0)
		{
			lpSceneMng.AddInstanceQue({ UNIT_ID::FOLLOWER,type ,(*pot).pos(),0 });
			followerList[_cursorNum]--;
		}
	}

	lpUI_Mng.AddUIQue({ 0,0,lpImageMng.GetID({IMG::MENU, STATE::NORMAL})[0]});
	// カーソル表示
	lpUI_Mng.AddUIQue({ MENU_OFSET_X + (_cursorNum / 3) * 160,MENU_OFSET_Y + (_cursorNum % 3) * 36,lpImageMng.GetID({ IMG::MENU_CURSOL, STATE::NORMAL })[0]});
	MakeBtnGuide(INPUT_ID::BTN_B, Vector2Dbl{ static_cast<double>(MENU_OFSET_X + (_cursorNum / 3) * 160 - 28) ,static_cast<double>(MENU_OFSET_Y + (_cursorNum % 3) * 36 - 4) });

	// 壺に入っているFollowerの数の描画
	for (int i = 0; i < 6; i++)
	{
		Text text;
		std::string str = { -126,static_cast<char>(79 + followerList[i] / 10),-126,static_cast<char>(79 + followerList[i] % 10) };
		text.MakeText(str, -8);
		lpUI_Mng.AddUIQue({MENU_OFSET_X + (i / 3) * 160 + 80, MENU_OFSET_Y + (i % 3) * 36, text.screen(),});
	}
}

void Player::throwPot(bool throwMode)
{
	_throwPot = throwMode;
}

void Player::DoDamage(int power)
{
	if (power >= 0)
	{
		lpSceneMng.AddSoundQue({ lpSoundMng.GetID(SOUND::DAMAGE)[0], 200, _pos.x, _pos.y });
		_shakeCount = 3;
	}
	Obj::DoDamage(power);
}

const Vector2Dbl & Player::ShakeOffset(void)
{
	return _shakeOffset;
}
