#pragma once
#include <vector>
#include <object\Obj.h>

struct BeeAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
private:
	double _effectRad;		// �̪�ėp�̊p�x
};
