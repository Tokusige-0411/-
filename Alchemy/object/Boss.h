#pragma once
#include "object/Obj.h"

#define HEIGHT_CNT_MAX 120.0
#define ADD_CNT_MAX	60;

class Boss :
	public Obj
{
public:
	Boss();
	Boss(sharedObj potObj);
	~Boss();
	void Update(std::vector<sharedObj>& objList) override;

private:
	void Init();
	bool AddBossMove(void);				// ｲﾝｽﾀﾝｽされて出現するときの動き
	bool AddBossHP(void);				// Hp
	void DrawHP(void)override;			// Hp描画用
	void BossDeath(void);				// ﾎﾞｽ死亡ｴﾌｪｸﾄ

	void AddBoss(void);					// ﾎﾞｽ出現

	int _bossHPImg[3];					// ﾎﾞｽのHP画像用
	int _heightCnt;						// 出現時の上から落ちてくるときに使う
	int _bossHPCount;					// HPちかちかしたい
	int _addCount;						// 出現時用のｶｳﾝﾄ
	double _addVec;						// 出現時のｽﾋﾟｰﾄﾞ
	bool _addFlag;						// 出現時用ﾌﾗｸﾞ
	STATE oldState;
	int deathScreen;					// 爆発がでかいから
	int _deathCnt;
};

