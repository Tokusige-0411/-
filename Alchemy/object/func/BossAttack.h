#pragma once
#include <vector>
#include <object\Obj.h>
#include <EFFECT.h>

enum class MOVE_TYPE
{
	CHARGE,
	BLAST
};

struct BossAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
private:
	// 各ｴﾌｪｸﾄの動き
	void Charge(Obj& obj);									// ﾁｬｰｼﾞ
	void Blast(Obj& obj, std::vector<sharedObj>& objList);	// 爆発

	MOVE_TYPE _move = MOVE_TYPE::CHARGE;					// 場所がないからここで初期化する
	int _chargeCnt = 0;										// ﾁｬｰｼﾞｶｳﾝﾄ
	Vector2Dbl _effectPos;									// ﾒﾃｵの座標
	EFFECT _type;						
};