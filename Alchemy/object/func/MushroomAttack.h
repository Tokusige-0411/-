#pragma once
#include <vector>
#include <object\Obj.h>
#include <EFFECT.h>

struct MushroomAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
private:
	EFFECT _id;													// ���ޯĂɂ���ďo���̪�Ă��Ⴄ
	double _effectRad;											// �̪�Ă̊p�x
};

