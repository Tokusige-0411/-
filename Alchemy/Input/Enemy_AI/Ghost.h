#pragma once
#include "../InputState.h"
#include "../AI_Ctl.h"

class Ghost :
	public InputState
{
public:
	Ghost(Obj& obj);
	~Ghost();
	void Update(std::vector<sharedObj>& objList) override;			// ����

private:
	Obj & _masterObj;
	AI_Ctl _aiCtl{ _masterObj, *this };
};

