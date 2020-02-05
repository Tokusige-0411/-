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
	// Še´Ìª¸Ä‚Ì“®‚«
	void Charge(Obj& obj);									// Á¬°¼Ş
	void Blast(Obj& obj, std::vector<sharedObj>& objList);	// ”š”­

	MOVE_TYPE _move = MOVE_TYPE::CHARGE;					// êŠ‚ª‚È‚¢‚©‚ç‚±‚±‚Å‰Šú‰»‚·‚é
	int _chargeCnt = 0;										// Á¬°¼Ş¶³İÄ
	Vector2Dbl _effectPos;									// ÒÃµ‚ÌÀ•W
	EFFECT _type;						
};