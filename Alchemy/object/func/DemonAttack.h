#pragma once
#include <vector>
#include <object\Obj.h>

enum class MOVE_TYPE
{
	CHARGE,
	BLAST
};

struct DemonAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
private:
	// 各ｴﾌｪｸﾄの動き
	void Charge(Obj& obj);									// ﾁｬｰｼﾞ
	void Blast(Obj& obj, std::vector<sharedObj>& objList);	// 爆発

	MOVE_TYPE _move = MOVE_TYPE::CHARGE;					// 場所がないからここで初期化する
	int _chargeCnt = 0;										// ﾁｬｰｼﾞｶｳﾝﾄ
};

