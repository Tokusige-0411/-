#pragma once
#include <vector>
#include <object\Obj.h>

struct PotAttack
{
	void operator ()(Obj& obj, std::vector<sharedObj>& objList);
};

