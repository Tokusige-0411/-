#pragma once
#include "BaseScene.h"
#include "SceneMng.h"
#include <object/Obj.h>
#include <Input/InputState.h>
#include<object/PlNum.h>
#include<vector>

class EntrySceneLocal :
	public BaseScene
{
public:
	EntrySceneLocal();
	~EntrySceneLocal();

	UniqueBase Update(UniqueBase own)override;

private:
	void Init(void);
	void DrawInit(void);
	void EntryStart(void);
	void EntryNormal(void);

	void (EntrySceneLocal::*func)(void);

	int _padnum;								 // 接続中のpad数
	int _entryPadnum;							 // エントリーしたpad数

	int _entrnum[2];							// エントリーした番号を保存するや～つ

	int _entryBG;								// エントリー背景
	int _start;									// PUSHSTARTﾒｯｾｰｼﾞ
	int _entry;
	int _entryImage[4][4];						// 操作キャラを表す画像
	int _skyimage;								// 背景・空
	int _playerScreen[2];						// player描画先ｽｸﾘｰﾝ
	int _arrow[2];								// 矢印Image
	int _b;
	int _a;
	PlNum _plNum[2];							// 各ﾌﾟﾚｲﾔｰnum保存
	bool _readyFlag[2];							// 準備完了ﾌﾗｸﾞ

	int _fadecolor;								// フェードの数値
	int _fadespeed;								// フェード速度
	int _cnt[2];								// ﾌﾚｰﾑｶｳﾝﾄ


	std::shared_ptr<InputState> _input[4];		// 入力管理クラス

	/*std::vector<std::shared_ptr<InputState>> _input;*/
};


