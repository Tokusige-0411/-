#pragma once
#include <vector>
#include "../Obj.h"

// Obj��STATE��HOLDEN(������Ă���)��Ԃ̎���Update�֐�
struct HoldenUpdate
{
	void operator()(Obj&, std::vector<sharedObj>& objList);
};

