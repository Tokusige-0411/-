#pragma once
#include <memory>
#include "Obj.h"
#include "PlNum.h"
#include <Message.h>
#include <Input/INPUT_ID.h>

#define MENU_OFSET_X 504
#define MENU_OFSET_Y 310

class NetWork;
struct FuncThrow;
struct FuncHold;
struct FuncPut;
struct PlayerHold;

class Player :
	public Obj
{
public:
	Player();
	Player(PlNum plNum, Vector2Dbl pos, int padNum,sharedObj potObj);
	Player(PlNum plNum, Vector2Dbl pos, sharedObj potObj);
	void Update(std::vector<sharedObj>& objList) override;
	PlNum plNum(void);
	static int GetpCnt(void);
	static void ReSetpCnt(void);
	int holdWeightMax(void);
	int GetHoldListSize(void);
	void MakeBtnGuide(INPUT_ID btn);						// ボタンガイドの画像を作りDrawQueを投げる
	void MakeBtnGuide(INPUT_ID btn, Vector2Dbl drawPos);	// 座標指定版
	bool throwPot(void);
	const int checkNum(void)const;
	void throwPot(bool throwMode);
	void DoDamage(int power) override;
	const Vector2Dbl& ShakeOffset(void);

	~Player();
private:
	friend NetWork;
	friend FuncHold;
	friend FuncThrow;
	friend FuncPut;
	friend PlayerHold;
	void Init(void);
	void DrawHP(void)override;
	void Menu(void);

	static bool _menuFlag[2];					// ﾒﾆｭｰを表示するかどうか
	bool _throwPot;					// 持ってるときに壺をターゲットにするかどうか
	int _holdWeightMax;				// 持てる重さ上限
	double _throwRange;				// 投げの距離
	int _playerHPImg[3];		// プレイヤー用のHP画像
	int _buttonID;
	int _cursorNum;					// メニュー開いてる時のカーソルの位置
	static int _playerIcon[4][2];	//
	static int pCnt;
	int _checkNum;						// そのﾌﾟﾚｲﾔｰがplayre何か
	PlNum _plNum;
	std::vector<sharedObj> _holdList;			// 現在持っているオブジェクト
	int _shakeCount;					// ｼｪｲｸする時間
	Vector2Dbl _shakeOffset;			// ｼｪｲｸした座標のｵﾌｾｯﾄ
};

