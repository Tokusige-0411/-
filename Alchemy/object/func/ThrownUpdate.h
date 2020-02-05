#pragma once
#include <vector>
#include "../Obj.h"

// ObjのSTATEがTHROWN(投げられている)状態の時のUpdate関数
struct ThrownUpdate
{
	void operator()(Obj&, std::vector<sharedObj>& objList);
};

