#pragma once
#include "../InputState.h"
#include "../AI_Ctl.h"

class Demon :
	public InputState
{
public:
	Demon(Obj& obj);
	~Demon();
	void Update(std::vector<sharedObj>& objList) override;			// ����

private:
	Obj & _masterObj;
	AI_Ctl _aiCtl{ _masterObj, *this };
};

