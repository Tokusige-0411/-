#pragma once
#include <vector>
#include "../Obj.h"

// Obj��STATE��THROWN(�������Ă���)��Ԃ̎���Update�֐�
struct ThrownUpdate
{
	void operator()(Obj&, std::vector<sharedObj>& objList);
};

