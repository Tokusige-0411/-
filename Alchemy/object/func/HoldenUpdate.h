#pragma once
#include <vector>
#include "../Obj.h"

// ObjのSTATEがHOLDEN(持たれている)状態の時のUpdate関数
struct HoldenUpdate
{
	void operator()(Obj&, std::vector<sharedObj>& objList);
};

